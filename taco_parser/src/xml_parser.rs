use serde::{Deserialize, Serialize};
use serde_xml_rs;
use std::collections::HashMap;
use std::fs::{read, read_to_string};
use std::path::Path as OsPath;
use std::path::PathBuf;

use crate::trail_parser::{self, TrailCoordinates};

#[cfg(test)]
#[path = "xml_parser_tests.rs"]
mod xml_parser_tests;

// #[derive(Debug, Deserialize)]
// pub struct OverlayData {
//     #[serde(rename = "MarkerCategory")]
//     pub marker_category: Option<MarkerCategory>,
//     #[serde(rename = "POIs")]
//     pois: POIArrayContainer,
// }

#[derive(Debug, Deserialize)]
pub struct OverlayData {
    #[serde(rename = "MarkerCategory", default = "default_marker_category")]
    pub marker_category: Vec<MarkerCategory>,
    #[serde(rename = "POIs")]
    pois: Option<POIArrayContainer>,
}

#[derive(Debug, Deserialize, Clone)]
pub struct MarkerCategory {
    name: String,
    #[serde(rename = "iconFile")]
    icon_file: Option<String>,
    #[serde(rename = "MarkerCategory", default = "default_marker_category")]
    children: Vec<MarkerCategory>,
    #[serde(rename = "heightOffset")]
    height_offset: Option<f32>,
}

fn default_marker_category() -> Vec<MarkerCategory> {
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

#[derive(Debug, Serialize, Clone)]
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
    if icon_file != "" {
        result.insert(new_prefix.to_string(), icon_file);
    }
    for node in right {
        construct_lookup_map(result, node, new_prefix.to_string());
    }
}

fn get_full_path(root: &OsPath, path: String) -> String {
    if path != "" {
        let full_path = root.join(path).into_os_string().into_string().unwrap();
        return full_path;
    }
    return path;
}

// Processes PoiItem and converts it into FinalResult. Mutates map of key = map_id, result = FinalResult
fn process_poi(
    mut acc: HashMap<u32, FinalResult>,
    item: &PoiItems,
    lookup: &HashMap<String, String>,
    subfolder: &OsPath,
) -> HashMap<u32, FinalResult> {
    // Replace POIs in path name to fix relative paths
    match item {
        // If it is a POI item
        PoiItems::POI(poi) => {
            let map_id = poi.map_id;
            let texture_rel_path = match get_texture(&lookup, poi) {
                Some(texture) => convert_windows_path(texture),
                None => "".to_string(),
            };
            let texture_full = get_full_path(subfolder, texture_rel_path);
            acc.entry(map_id)
                .and_modify(|value| {
                    value.icons.push(Icon {
                        position: [poi.xpos, poi.ypos, poi.zpos],
                        texture: texture_full.clone(),
                    });
                })
                .or_insert(FinalResult {
                    icons: vec![Icon {
                        position: [poi.xpos, poi.ypos, poi.zpos],
                        texture: texture_full.clone(),
                    }],
                    paths: Vec::new(),
                });
        }
        // If it is a Trail item
        PoiItems::TrailMetadata(trail_metadata) => {
            // Read trail .trl file
            let texture_rel_path = convert_windows_path((&trail_metadata.texture).to_string());
            let texture_full = get_full_path(subfolder, texture_rel_path);

            let trail_file = &trail_metadata.trail_data;
            let byte_vector: &[u8] = &read(subfolder.join(trail_file)).unwrap();
            let parsed_trail = trail_parser::parse_trail(byte_vector).unwrap().1;
            let trail_coordinates = parsed_trail.coordinates;
            let points: Vec<[f32; 3]> = trail_coordinates
                .iter()
                .map(|x| unwrap_trail_coordinates(x))
                .collect();
            let trail_points = Path {
                points,
                texture: texture_full,
            };
            acc.entry(parsed_trail.map_id as u32)
                .and_modify(|value| value.paths.push(trail_points.clone()))
                .or_insert(FinalResult {
                    icons: Vec::new(),
                    paths: vec![trail_points],
                });
        }
    }
    return acc;
}

fn get_categorydata_file(folder_name: &String) -> PathBuf {
    let category_file_name = "categorydata.xml";
    let category_file = OsPath::new(&folder_name).join(category_file_name);
    return category_file;
}

// Basic escaping of &. No escaping for other characters for now
fn escape_xml_str(input: String) -> String {
    return input.replace("&", "&amp;");
}

pub fn process_taco_data(folder_name: String, xml_file: String) -> HashMap<u32, FinalResult> {
    let mut lookup = HashMap::new();
    let root_folder = folder_name.replace("POIs", "");
    // Attempt to load in `categorydata.xml` first
    let categorydata_file = get_categorydata_file(&root_folder);
    if categorydata_file.exists() {
        let default_category_content = escape_xml_str(read_to_string(categorydata_file).unwrap());
        let default_category_parsed = parse_xml(&default_category_content);
        for marker in default_category_parsed.marker_category {
            construct_lookup_map(&mut lookup, marker, "".to_string());
        }
    }

    // Optionally load markers in
    let subfolder = OsPath::new(&folder_name);
    let contents = escape_xml_str(read_to_string(OsPath::new(subfolder).join(xml_file)).unwrap());
    let xml_parsed = parse_xml(&contents);
    for marker in xml_parsed.marker_category {
        construct_lookup_map(&mut lookup, marker, "".to_string());
    }

    // POI Array stuff
    let poi_array = match xml_parsed.pois {
        Some(poi_array) => poi_array.poi_array,
        None => Vec::new(),
    };

    // let poi_array = xml_parsed.pois.poi_array;
    let process_poi_closure = |acc: HashMap<u32, FinalResult>, item: &PoiItems| {
        process_poi(acc, item, &lookup, OsPath::new(&root_folder))
    };

    let empty_map: HashMap<_, _> = HashMap::new();
    let result = poi_array.iter().fold(empty_map, process_poi_closure);
    return result;
}
