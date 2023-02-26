import os

import xml.etree.ElementTree as ET
import struct
import shutil
from typing import *
import json
import sys
from enum import Enum
from tabulate import tabulate

output_dir = ""

source_path: str = ""
target_path: str = ""

def main():
    if len(sys.argv) != 3:
        print(f"USAGE: {sys.argv[0]} SRC_DIR DEST_DIR")
        sys.exit(1)
        
    global error_in_new_file
    global files_with_errors
    global error_total
    global source_path
    global target_path

    source_path = sys.argv[1]
    target_path = sys.argv[2]

    if not os.path.exists(target_path):
        os.makedirs(os.path.join(target_path, "data"))

    for file in os.scandir(source_path):
        if(file.name.endswith(".xml")):
            full_path: str = os.path.join(source_path, file.name)
            error_in_new_file = True
            convert_markers(full_path, target_path)
            
    print_error_stats()

################################################################################
# Quick error printing logic
################################################################################
error_location = ""
error_in_new_file: bool = True
error_total = 0
files_with_errors = list()

class ErrorType(Enum):
    INFO = 0
    WARNING = 1
    ERROR = 2
    
def eprint(*args, error_level = ErrorType.WARNING):
    global error_in_new_file
    global error_total
    error_total += 1
    
    if error_in_new_file:
        print(error_level.name + " in file \"" + error_location + "\": ")
        files_with_errors.append(error_location)
        error_in_new_file = False

    for arg in list(args):
        if(type(arg) != ErrorType):
            print("    " + str(arg))
    print("    Skipping file...", end="\n\n")
    
    if(error_level == ErrorType.ERROR):
        print("Fatal error, exiting.")
        exit()

def print_error_stats():
    global error_total
    global files_with_errors
    print("Conversion finished, found " + str(error_total) + " errors in " + str(len(files_with_errors)) + " files:\n    ")
    for file in files_with_errors:
        print(str(file))
    
        
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
        "alpha", # Ignored
        "animSpeed", # Ignored
        "texture",
        "trailScale", # Ignored
        "scaleOnMapWithZoom", # Ignored
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

    if "texture" in attribs:
        limited_attribs["texture"] = attribs["texture"]

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
    global error_location
    
    error_location = str(xml_path)
    
    try:
        tree = ET.parse(xml_path)
    except Exception as e:
        eprint("Error trying to parse XML, likely incorrectly formatted:", str(e), error_level=ErrorType.ERROR)
    else:
        root = tree.getroot()
    
    # warnings
    if root.tag != "OverlayData":
        eprint("Root not OverlayData", root.tag)

    if root.attrib != {}:
        eprint("Root has unknown attribs", root.attrib)

    if len(root) != 2:
        eprint("Root has {} children instead of the expected 2".format(len(root)))
        return

    if root[0].tag != "MarkerCatego6ry":
        eprint("First element of root is {} not MarkerCategory".format(root[0].tag))
    else:
        marker_metadata = parse_marker_category(root[0])

    if root[1].tag != "POIs":
        eprint("Second element of root is {} not POIs".format(root[1].tag))
    else:
        burrito_marker_data = parse_icons_and_paths(root[1], marker_metadata, os.path.dirname(xml_path))

    with open(os.path.join(output_dir, os.path.basename(xml_path)[:-4] + ".json"), 'w') as f:
        json.dump(burrito_marker_data, f)


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
        "resetLength", # Ignored
        "info", # Ignored
        "infoRange", # Ignored
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
                eprint("Trying to use type", attribs["type"], "which does not exist as a MarkerCategory")
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

                texture = ""

                if attribs["type"] in marker_metadata:
                    for point in point_subsection:
                        old_point = point[1]

                        point[1] += marker_metadata[attribs["type"]]["height_offset"]

                    if "texture" in marker_metadata[attribs["type"]]:
                        texture = marker_metadata[attribs["type"]]["texture"]
                    if "texture" in attribs:
                        texture = attribs["texture"]

                        # print(old_point, marker_metadata[attribs["type"]]["height_offset"], point[1])
                else:
                    eprint("Type Not Found", attribs["type"])

                if texture == "":
                    eprint("No texture found")
                    pass

                burrito_marker_data[map_id]["paths"].append(
                    {
                        "texture": copyimage(texture),
                        "points": point_subsection
                    }
                )


        else:
            eprint("First element of root is {} not POIs".format(root[1].tag))

    return burrito_marker_data

image_names = {}

def copyimage(image_path):
    # shutil.copy()

    if image_path == "":
        eprint("No Image File Specified")
        return ""

    image_path = os.path.join(source_path, image_path)
    new_name = os.path.join("data", os.path.basename(image_path))

    if new_name in image_names and image_names[new_name] != image_path:
        print("Duplicate image name")
    image_names[new_name] = image_path


    if not os.path.exists(image_path):
        eprint("Missing Icon File:", image_path)
        return ""

    shutil.copy(
        image_path,
        os.path.join(target_path, new_name)
    )


    return new_name

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
