use gdnative::prelude::*;
use std::path::Path;

mod trail_parser;
mod xml_parser;

fn taco_to_json_str(folder_name: String, xml_file: String) -> Result<String, serde_xml_rs::Error> {
    let parsed = xml_parser::process_taco_data(folder_name, xml_file);
    let result = match parsed {
        Ok(x) => Ok(serde_json::to_string(&x).unwrap()),
        Err(e) => Err(e),
    };
    return result;
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
    fn parse_taco_xml(&self, _owner: &Node, file_path: String) -> (String, String) {
        let errored_payload = "{}".to_string();
        let full_path = Path::new(&file_path);
        let folder = match full_path.parent() {
            Some(path) => path.to_str().unwrap().to_string(),
            None => return (errored_payload, "No parent folder".to_string()),
        };
        let xml_file = full_path.file_name().unwrap().to_str().unwrap();
        let result = taco_to_json_str(folder.to_string(), xml_file.to_string());
        // return tuple of (payload, err_message) back to Godot.
        match result {
            Ok(x) => return (x, "".to_string()),
            Err(e) => return (errored_payload, e.to_string()),
        };
    }
}

// Macro that creates the entry-points of the dynamic library.
godot_init!(init);
