use serde::{Deserialize, Serialize};
use serde_xml_rs;
use std::collections::HashMap;
use std::fs::{read, read_to_string};
use std::path::Path as OsPath;

use crate::trail_parser::{self, TrailCoordinates};

#[derive(Debug, Deserialize)]
pub struct OverlayData {
    #[serde(rename = "MarkerCategory")]
    marker_category: MarkerCategory,
    #[serde(rename = "POIs")]
    pois: POIArrayContainer,
}

#[derive(Debug, Deserialize)]
pub struct MarkerCategoryArrayContainer {
    name: String,
    #[serde(rename = "$value")]
    marker_category_array: Vec<MarkerCategory>,
}

#[derive(Debug, Deserialize)]
pub struct MarkerCategory {
    name: String,
    #[serde(rename = "iconFile")]
    icon_file: Option<String>,
    #[serde(rename = "MarkerCategory")]
    marker_category: Option<MarkerCategoryArrayContainer>,
    #[serde(rename = "heightOffset")]
    height_offset: Option<f32>
}

#[derive(Debug, Deserialize)]
pub struct POIArrayContainer {
    #[serde(rename = "$value")]
    poi_array: Vec<PoiItems>,
}

#[derive(Debug, Deserialize)]
struct POI {
    #[serde(rename = "MapID")]
    map_id: u32,
    xpos: f32,
    ypos: f32,
    zpos: f32,
    #[serde(rename = "iconFile")]
    icon_file: Option<String>,
    #[serde(rename = "heightOffset")]
    height_offset: Option<f32>,
    #[serde(rename = "type")]
    type_: String
}

#[derive(Debug, Deserialize)]
struct TrailMetadata {
    #[serde(rename = "trailData")]
    trail_data: String,
    texture: String,
    color: String,
    #[serde(rename = "iconFile")]
    icon_file: Option<String>
}

#[derive(Debug, Deserialize)]
enum PoiItems {
    POI(POI),
    #[serde(rename = "Trail")]
    TrailMetadata(TrailMetadata)
}


#[derive(Debug, Serialize)]
pub struct Converted {
    icons: Vec<Icon>,
    paths: Vec<Path>
}

#[derive(Debug, Serialize, Clone)]
struct Path {
    texture: String,
    points: Vec<[f32; 3]>
}

#[derive(Debug, Serialize, Clone)]
struct Icon {
    position: [f32; 3],
    texture: String
}

//https://docs.rs/serde-xml-rs/0.5.1/serde_xml_rs/
pub fn parse_xml(xml: &str ) -> OverlayData {
    let data: OverlayData = serde_xml_rs::from_str(xml).unwrap();
    data
}

// if icon_file exists in MarkerCategory, add to map
fn add_to_hashmap(mut acc: HashMap<String, String>, item: &MarkerCategory, key_prefix: String) -> HashMap<String, String> {
    let key = [key_prefix, item.name.to_string()].join(".");
    let icon_file = item.icon_file.clone();
    match icon_file {
        Some(icon) => acc.insert(key, icon),
        None => None,
    };
    return acc
}

fn convert_windows_path(path: String) -> String {
    path.replace("\\", "/")
}

fn get_texture(lookup: &HashMap<String, String>, poi: &POI) -> Option<String> {
    let texture = match lookup.get(&poi.type_) {
        Some(x) => Some(x.to_string()),
        None if poi.icon_file.is_some() => poi.icon_file.clone(),
        None => None
    };
    return texture
}

fn unwrap_trail_coordinates(input: &TrailCoordinates) -> [f32; 3] {
    return [input.xpos, input.ypos, input.zpos]
}

pub fn process_taco_data(folder_name: String, xml_file: String) -> HashMap<u32, Converted> {
    let folder = OsPath::new(&folder_name);
    let contents = read_to_string(OsPath::new(folder).join(xml_file)).unwrap();
    let xml_parsed = parse_xml(&contents);

    let root_marker_category = xml_parsed.marker_category;

    let marker_category_array_container = root_marker_category.marker_category.unwrap();

    let marker_category_array = marker_category_array_container.marker_category_array;
    let key_prefix = [root_marker_category.name, marker_category_array_container.name].join(".");

    let add_to_hashmap_with_prefix = | acc: HashMap<_, _>, item: &MarkerCategory| {add_to_hashmap(acc, item, key_prefix.clone())};
    let empty_map: HashMap<_, _> = HashMap::new();
    let lookup = marker_category_array.iter().fold(empty_map, add_to_hashmap_with_prefix);

    let poi_array = xml_parsed.pois.poi_array;
    let folder_function = | mut acc: HashMap<u32, Converted>, item: &PoiItems | -> HashMap<u32, Converted> {
        match item {
            PoiItems::POI(poi)=> { 
                let map_id = poi.map_id;
                let array_ = acc.get(&map_id);
                match array_ {
                    Some(x_) => {
                        let mut res = x_.icons.clone();
                        // get texture. Make it a function?
                        let texture = convert_windows_path(get_texture(&lookup, poi).unwrap());
                        res.push(Icon{position: [poi.xpos, poi.ypos, poi.zpos], texture});
                        acc.insert(map_id, Converted{icons: res, paths: Vec::new()});
                    }
                    _ => {
                        let mut vec_ = Vec::new();
                        let texture = convert_windows_path(get_texture(&lookup, poi).unwrap());
                        vec_.push(Icon{position: [poi.xpos, poi.ypos, poi.zpos], texture});
                        acc.insert(map_id, Converted{icons: vec_, paths: Vec::new()});
                    }
                }
            }
            PoiItems::TrailMetadata(trail_metadata) => {
                // Read trail .trl file
                let trail_file = &trail_metadata.trail_data;
                let texture = &trail_metadata.texture;
                let byte_vector: &[u8] = &read(folder.join(trail_file)).unwrap();
                let parsed_trail = trail_parser::parse_trail(byte_vector).unwrap().1;
                let current_retrieved = acc.get(&(parsed_trail.map_id as u32));
                let trail_coordinates = parsed_trail.coordinates;
                let points: Vec<[f32; 3]> = trail_coordinates.iter().map(|x| unwrap_trail_coordinates(x)).collect();
                match current_retrieved {
                    Some(current_item) => {
                        let trail_points = Path{points, texture: convert_windows_path(texture.to_string())};
                        let mut paths = (current_item.paths).clone();
                        paths.push(trail_points);
                        let icons = current_item.icons.clone();
                        acc.insert(parsed_trail.map_id as u32, Converted{icons, paths});
                    }
                    None => (),
                }
            }
        }
        return acc;
    };

    let empty_map: HashMap<_, _> = HashMap::new();
    let result = poi_array.iter().fold(empty_map, folder_function);
    return result
}

// Convert to map{ map_id: 
    // {
    //     "1330": {
    //         "icons": [
    //             {
    //                 "position": [
    //                     -246.114,
    //                     19.9021,
    //                     -586.05
    //                 ],
    //                 "texture": "data/coral.png"
    //             },]
    //          "paths": [{"texture": "", points: [[1, 2, 3], ...]}]