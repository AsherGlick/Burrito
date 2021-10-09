use gdnative::prelude::*;
use std::path::Path;

mod trail_parser;
mod xml_parser;

fn taco_to_json_str(folder_name: String, xml_file: String) -> String {
    let parsed = xml_parser::process_taco_data(folder_name, xml_file);
    let json_string = serde_json::to_string(&parsed).unwrap();
    return json_string;
}

#[derive(NativeClass)]
#[inherit(Node)]
pub struct TacoParser;
// Function that registers all exposed classes to Godot
fn init(handle: InitHandle) {
    handle.add_class::<TacoParser>();
}

impl TacoParser {
    /// The "constructor" of the class.
    fn new(_owner: &Node) -> Self {
        TacoParser
    }
}

#[methods]
impl TacoParser {
    #[export]
    fn _ready(&self, _owner: &Node) {
        godot_print!("Hello,X11!");
        return;
    }

    #[export]
    fn parse_taco_xml(&self, _owner: &Node, file_path: String) -> String {
        let full_path = Path::new(&file_path);
        let folder = full_path.parent().unwrap().to_str().unwrap();
        let xml_file = full_path.file_name().unwrap().to_str().unwrap();
        let result = taco_to_json_str(folder.to_string(), xml_file.to_string());
        return result;
    }
}

// Macro that creates the entry-points of the dynamic library.
godot_init!(init);
