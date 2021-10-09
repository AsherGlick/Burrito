use serde::{Deserialize, Serialize};
use serde_xml_rs;
use std::collections::HashMap;
use std::fs::{read, read_to_string};
use std::path::Path as OsPath;

use crate::trail_parser::{self, TrailCoordinates};

#[cfg(test)]
#[path = "xml_parser_tests.rs"]
mod xml_parser_tests;

#[derive(Debug, Deserialize)]
pub struct OverlayData {
    #[serde(rename = "MarkerCategory")]
    pub marker_category: MarkerCategory,
    #[serde(rename = "POIs")]
    pois: POIArrayContainer,
}

#[derive(Debug, Deserialize, Clone)]
pub struct MarkerCategory {
    name: String,
    #[serde(rename = "iconFile")]
    icon_file: Option<String>,
    #[serde(rename = "MarkerCategory", default = "default_children")]
    children: Vec<MarkerCategory>,
    #[serde(rename = "heightOffset")]
    height_offset: Option<f32>,
}

fn default_children() -> Vec<MarkerCategory> {
    return Vec::new();
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
    type_: String,
}

#[derive(Debug, Deserialize)]
struct TrailMetadata {
    #[serde(rename = "trailData")]
    trail_data: String,
    texture: String,
    // color: String,
    #[serde(rename = "iconFile")]
    icon_file: Option<String>,
}

#[derive(Debug, Deserialize)]
enum PoiItems {
    POI(POI),
    #[serde(rename = "Trail")]
    TrailMetadata(TrailMetadata),
}

#[derive(Debug, Serialize)]
pub struct FinalResult {
    icons: Vec<Icon>,
    paths: Vec<Path>,
}

#[derive(Debug, Serialize, Clone)]
struct Path {
    texture: String,
    points: Vec<[f32; 3]>,
}

#[derive(Debug, Serialize, Clone)]
struct Icon {
    position: [f32; 3],
    texture: String,
}

//https://docs.rs/serde-xml-rs/0.5.1/serde_xml_rs/
pub fn parse_xml(xml: &str) -> OverlayData {
    let data: OverlayData = serde_xml_rs::from_str(xml).unwrap();
    data
}

fn convert_windows_path(path: String) -> String {
    path.replace("\\", "/")
}

fn get_texture(lookup: &HashMap<String, String>, poi: &POI) -> Option<String> {
    let texture = match lookup.get(&poi.type_) {
        Some(x) => Some(x.to_string()),
        None if poi.icon_file.is_some() => poi.icon_file.clone(),
        None => None,
    };
    return texture;
}

fn unwrap_trail_coordinates(input: &TrailCoordinates) -> [f32; 3] {
    return [input.xpos, input.ypos, input.zpos];
}

fn construct_lookup_map(
    result: &mut HashMap<String, String>,
    node: MarkerCategory,
    prefix: String,
) -> () {
    // let mut result: HashMap<String, String> = HashMap::new();
    // InOrderTraversal?
    let size = node.children.len();
    let split = 2;
    let mut left = node.children;
    let right = if size >= split {
        left.split_off(split)
    } else {
        vec![]
    };
    let inherited_prefix = prefix;
    let node_label = node.name.to_string();
    let new_prefix = if inherited_prefix != "" {
        [inherited_prefix, node_label].join(".")
    } else {
        node_label
    };
    let icon_file = match node.icon_file.clone() {
        Some(icon_file) => icon_file,
        None => "".to_string(),
    };
    for node in left {
        construct_lookup_map(result, node, new_prefix.to_string());
    }
    result.insert(new_prefix.to_string(), icon_file);
    for node in right {
        construct_lookup_map(result, node, new_prefix.to_string());
    }
}

// Processes PoiItem and converts it into FinalResult. Mutates map of key = map_id, result = FinalResult
fn process_poi(
    mut acc: HashMap<u32, FinalResult>,
    item: &PoiItems,
    lookup: &HashMap<String, String>,
    subfolder: &OsPath,
) -> HashMap<u32, FinalResult> {
    match item {
        // If it is a POI item
        PoiItems::POI(poi) => {
            let map_id = poi.map_id;
            let array_ = acc.get(&map_id);
            match array_ {
                Some(x_) => {
                    let mut res = x_.icons.clone();
                    let texture = convert_windows_path(get_texture(&lookup, poi).unwrap());
                    res.push(Icon {
                        position: [poi.xpos, poi.ypos, poi.zpos],
                        texture,
                    });
                    acc.insert(
                        map_id,
                        FinalResult {
                            icons: res,
                            paths: Vec::new(),
                        },
                    );
                }
                None => {
                    let mut vec_ = Vec::new();
                    let texture = convert_windows_path(get_texture(&lookup, poi).unwrap());
                    vec_.push(Icon {
                        position: [poi.xpos, poi.ypos, poi.zpos],
                        texture,
                    });
                    acc.insert(
                        map_id,
                        FinalResult {
                            icons: vec_,
                            paths: Vec::new(),
                        },
                    );
                }
            }
        }
        // If it is a Trail item
        PoiItems::TrailMetadata(trail_metadata) => {
            // Read trail .trl file
            let trail_file = &trail_metadata.trail_data;
            let texture = &trail_metadata.texture;
            let byte_vector: &[u8] = &read(subfolder.join(trail_file)).unwrap();
            let parsed_trail = trail_parser::parse_trail(byte_vector).unwrap().1;
            let current_retrieved = acc.get(&(parsed_trail.map_id as u32));
            let trail_coordinates = parsed_trail.coordinates;
            let points: Vec<[f32; 3]> = trail_coordinates
                .iter()
                .map(|x| unwrap_trail_coordinates(x))
                .collect();
            match current_retrieved {
                Some(current_item) => {
                    let trail_points = Path {
                        points,
                        texture: convert_windows_path(texture.to_string()),
                    };
                    let mut paths = (current_item.paths).clone();
                    paths.push(trail_points);
                    let icons = current_item.icons.clone();
                    acc.insert(parsed_trail.map_id as u32, FinalResult { icons, paths });
                }
                None => (),
            }
        }
    }
    return acc;
}

pub fn process_taco_data(folder_name: String, xml_file: String) -> HashMap<u32, FinalResult> {
    // marker stuff to construct the lookup map
    let subfolder = OsPath::new(&folder_name);
    let contents = read_to_string(OsPath::new(subfolder).join(xml_file)).unwrap();
    let xml_parsed = parse_xml(&contents);

    let marker_categories = xml_parsed.marker_category;
    let mut lookup = HashMap::new();
    construct_lookup_map(&mut lookup, marker_categories, "".to_string());

    // POI Array stuff
    let poi_array = xml_parsed.pois.poi_array;
    let process_poi_closure = |acc: HashMap<u32, FinalResult>, item: &PoiItems| {
        process_poi(acc, item, &lookup, subfolder)
    };

    let empty_map: HashMap<_, _> = HashMap::new();
    let result = poi_array.iter().fold(empty_map, process_poi_closure);
    return result;
}
