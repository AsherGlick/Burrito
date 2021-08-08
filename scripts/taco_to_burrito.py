import os

import xml.etree.ElementTree as ET
import struct

from typing import *
import json

output_dir = ""

def main():
    source_path: str = "tekkit_workshop_taco_markers"

    for file in os.scandir(source_path):
        if(file.name.endswith(".xml")):
            full_path: str = os.path.join(source_path, file.name)
            convert_markers(full_path, "tekkit_workshop_burrito_markers")

################################################################################
# Quick error printing logic that only prints the title of a section if there
# are any errors in it. Could be handled better by returning errors from the
# function but that is more work to implement then this is.
################################################################################
eprint_title: str = ""
eprint_trigger: bool = False
def set_eprint_title(title: str):
    global eprint_title
    global eprint_trigger
    eprint_title = title
    eprint_trigger = False
def eprint(*args, **kwargs):
    global eprint_trigger
    if not eprint_trigger:
        eprint_trigger = True
        print(eprint_title)
    print(*args, **kwargs)


################################################################################
#
################################################################################
def parse_marker_category(marker_category_node, base=""):
    required_keys = set(["DisplayName", "name" ])
    allowed_keys = set([
        "iconFile",
        "iconSize", # Ignored
        "heightOffset",
        "minSize", # Ignored
        "mapDisplaySize", # Ignored
        "fadeNear", # Ignored
        "fadeFar",  # Ignored
        "inGameVisibility", # Ignored
        "miniMapVisibility", # Ignored
        "mapVisibility",     # Ignored
        "IsSeparator", # Ignored - Only useful for menu system not being implemented
        "behavior", # Ignored
        "toggleCategory", # Ignored - Used for unimplemented functionality
        "resetLength", # Ignored
    ])


    metadata_tree = {}

    if marker_category_node.tag != "MarkerCategory":
        eprint("MarkerCategory Child not  MarkerCategory", marker_category_node.tag)

    attribs = marker_category_node.attrib
    for present_key in attribs:
        if present_key not in required_keys and present_key not in allowed_keys:
            eprint("key {} found that is not allowed".format(present_key))

    for required_key in required_keys:
        if required_key not in attribs:
            eprint("key {} missing from marker category".format(required_key))


    limited_attribs = {}
    if "iconFile" in attribs:
        limited_attribs["icon_file"] = attribs["iconFile"]

    if "iconSize" in attribs:
        limited_attribs["icon_size"] = attribs["iconSize"]

    if "heightOffset" in attribs:
        limited_attribs["height_offset"] = attribs["heightOffset"]
    else:
        limited_attribs["height_offset"] = 0


    name = attribs["name"]

    metadata_tree[base + name] = limited_attribs

    for child in marker_category_node:
        subtree = parse_marker_category(child, base + name + ".")
        for elem in subtree:
            metadata_tree[elem] = subtree[elem]

    return metadata_tree

################################################################################
# 
################################################################################
def convert_markers(xml_path: str, output_dir: str):
    set_eprint_title(xml_path)
    tree = ET.parse(xml_path)
    root = tree.getroot()

    if root.tag != "OverlayData":
        eprint("Root not OverlayData", root.tag)

    if root.attrib != {}:
        eprint("Root has unknown attribs", root.attrib)

    if len(root) != 2:
        eprint("Root has {} children instead of the expected 2".format(len(root)))

    if root[0].tag != "MarkerCategory":
        eprint("First element of root is {} not MarkerCategory".format(root[0].tag))
    else:
        marker_metadata = parse_marker_category(root[0])

    # print(marker_metadata)

    if root[1].tag != "POIs":
        eprint("First element of root is {} not POIs".format(root[1].tag))
    else:
        burrito_marker_data = parse_icons_and_paths(root[1], marker_metadata, os.path.dirname(xml_path))


    with open(os.path.join(output_dir, os.path.basename(xml_path)[:-4] + ".json"), 'w') as f:
        json.dump(burrito_marker_data, f)

    # return burrito_marker_data
    # print(marker_metadata)




# class icon():


# class path():

# @dataclass
# class mapdata():
#     icons: List[icon]
#     paths: List[path]

def aprox_zero(a:float):
    return abs(a) <= 1e-09


def parse_icons_and_paths(poi_node, marker_metadata, dirname=""):

    required_poi_attrib = set([
        "GUID", # Ignored
        "xpos",
        "ypos",
        "zpos",
        "MapID",
        "type",
    ])
    allowed_poi_attrib = set([
        "fadeFar", # Ignored
        "minSize", # Ignored
        "iconSize", # Ignored
        "heightOffset",
        "iconFile", 
        "mapDisplaySize", # Ignored
        "fadeNear", # Ignored
        "behavior", # Ignored
        "resetLength" # Ignored
    ])

    required_trail_attrib = set([
        "texture",
        "animSpeed", # Ignored
        "trailData",
        "fadeFar", # Ignored
        "GUID", # Ignored
        "type", #Ignored TODO: maybe should not ignore?
    ])
    allowed_trail_attrib = set([
        "mapDisplaySize",  # Ignored
        "mapVisibility",  # Ignored
        "miniMapVisibility",  # Ignored
        "fadeNear",  # Ignored
    ])

    burrito_marker_data = {}


    for child in poi_node:
        if len(child) > 0:
            eprint("POI element has children")

        if child.tag == "POI":
            attribs = child.attrib
            for present_key in attribs:
                if present_key not in required_poi_attrib and present_key not in allowed_poi_attrib:
                    eprint("key {} found that is not allowed".format(present_key))

            for required_key in required_poi_attrib:
                if required_key not in attribs:
                    eprint("key {} missing from marker category".format(required_key))

            map_id = int(attribs["MapID"])
            
            if map_id not in burrito_marker_data:
                burrito_marker_data[map_id] = {"icons":[], "paths":[]}

            position: List[float] = [float(attribs["xpos"]), float(attribs["ypos"]), float(attribs["zpos"])]
            icon_path = ""

            if attribs["type"] in marker_metadata:
                position[1] += float(marker_metadata[attribs["type"]]["height_offset"])
                if "icon_file" in marker_metadata[attribs["type"]]:
                    icon_path = marker_metadata[attribs["type"]]["icon_file"]
                if "iconFile" in attribs:
                    icon_path = attribs["iconFile"]
            else:
                eprint("Type Not Found", attribs["type"])

            if icon_path == "":
                eprint("No Icon Found")
                pass
            else:
                # print("icon found")
                pass


            burrito_marker_data[map_id]["icons"].append({
                "position": position,
                "texture": copyimage(icon_path)
            })

        elif child.tag == "Trail":
            attribs = child.attrib
            for present_key in attribs:
                if present_key not in required_trail_attrib and present_key not in allowed_trail_attrib:
                    eprint("key {} found that is not allowed".format(present_key))

            for required_key in required_trail_attrib:
                if required_key not in attribs:
                    eprint("key {} missing from marker category".format(required_key))

            map_id, points = open_trail_format(os.path.join(dirname, attribs["trailData"]))


            point_subsections = [[]]
            for point in points:
                if aprox_zero(point[0]) and aprox_zero(point[1]) and aprox_zero(point[2]):
                    point_subsections.append([])
                else:
                    point_subsections[-1].append(point)

            for point_subsection in point_subsections:

                if map_id not in burrito_marker_data:
                    burrito_marker_data[map_id] = {"icons":[], "paths":[]}

                if attribs["type"] in marker_metadata:
                    for point in point_subsection:
                        old_point = point[1]

                        point[1] += marker_metadata[attribs["type"]]["height_offset"]

                        # print(old_point, marker_metadata[attribs["type"]]["height_offset"], point[1])
                else:
                    eprint("Type Not Found", attribs["type"])


                burrito_marker_data[map_id]["paths"].append(
                    {
                        "texture": copyimage(attribs["texture"]),
                        "points": point_subsection
                    }
                )


        else:
            eprint("First element of root is {} not POIs".format(root[1].tag))

    return burrito_marker_data



def copyimage(image_path):
    return image_path


def open_trail_format(trl_path: str) -> Tuple[int, List[float]]:
    with open(trl_path, 'rb') as f:
        file_bytes = f.read()

    version = struct.unpack("<i", memoryview(file_bytes)[0:4])[0] # Ignored
    map_id = struct.unpack("<i", memoryview(file_bytes)[4:8])[0]

    index = 8

    points = []
    while index < len(file_bytes):
        pointx, pointy, pointz = struct.unpack("<fff", memoryview(file_bytes)[index:index+12])[0:3]
        index += 12
        points.append([pointx, pointy, pointz])

    return map_id, points


if __name__ == "__main__":
    main()
