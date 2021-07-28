import json
import os
import requests

map_to_ingame_scale = 1.64042

maps_info = {}


def get_maps_info():
    global maps_info

    if maps_info != {}:
        return maps_info

    if os.path.exists("mapsinfo.json"):
        with open("mapsinfo.json") as f:
            maps_info = json.load(f)

    session = requests.Session()
    
    map_ids = json.loads(session.get("https://api.guildwars2.com/v2/maps").text)
    for map_id in map_ids:
        map_id = str(map_id)
        if map_id in maps_info:
            continue

        print(map_id)

        response_text = session.get("https://api.guildwars2.com/v2/maps/"+str(map_id)).text

        map_data = json.loads(response_text)
        print(map_data)

        if map_data == {"text": "too many requests"}:
            continue

        maps_info[map_id] = map_data

        with open("mapsinfo.json", "w") as f:
            json.dump( maps_info, f)

    return maps_info


def get_map_id(map_name):
    found_ids = []
    maps_info = get_maps_info()

    for map_id in maps_info:
        map_info = maps_info[map_id]
        if(map_info["name"] == map_name):
            found_ids.append(map_id)

    if len(found_ids) != 1:
        print("Should have found 1 id for {}, instead found {}. {}", map_name, len(found_ids), found_ids)

    return found_ids[0]


def main():
    with open("tw_mc_coretyria.json") as f:
        tekkit_map_completion = json.load(f)

    with open("heart_locations.json") as f:
        heart_locations = json.load(f)


    for map_name in heart_locations:

        map_id = get_map_id(map_name)

        string_points = heart_locations[map_name]
        points = []
        for string_point in string_points:
            points.append(json.loads(string_point))
        map_api = get_maps_info()[str(map_id)]

        areas = []
        for point_list in points:
            area_points = []
            for point in point_list:
                print(point)
                x, y, z = convert_map_to_ingame_point(point, map_api)
                area_points.append([x,y,z])

            areas.append({
                "points": area_points,
                "texture":"/home/vault/Code/projects/Burrito/tekkit_workshop_burrito_markers/area.png"
            })

        if map_id in tekkit_map_completion:
            tekkit_map_completion[map_id]["paths"] += areas

    with open("iggy_tw_mc_coretyria.json", "w") as f:
        json.dump(tekkit_map_completion, f)


def convert_map_to_ingame_point(map_point, map_api):
    map_x = map_point[0]
    map_y = map_point[1]

    map_size = map_api["continent_rect"]
    map_origin_x = map_size[0][0]
    map_origin_y = map_size[0][1]

    map_size_x = map_size[1][0] - map_origin_x
    map_size_y = map_size[1][1] - map_origin_y

    map_offset_x = (map_api["map_rect"][1][0]+map_api["map_rect"][0][0]) / 24 / 2
    map_offset_y = (map_api["map_rect"][1][1]+map_api["map_rect"][0][1]) / 24 / 2

    ingame_x = (map_x - map_origin_x - map_size_x/2 + map_offset_x) / map_to_ingame_scale
    ingame_y = 29
    ingame_z = (map_y - map_origin_y - map_size_y/2 - map_offset_y) / map_to_ingame_scale

    return(ingame_x, ingame_y, -ingame_z)


if __name__ == "__main__":
    main()

