use breadx::{
    auto::xproto::ATOM_WM_TRANSIENT_FOR,
    traits::{PropMode, PropertyFormat, PropertyType, ATOM_WM_NAME},
    Display, DisplayBase, DisplayConnection, Window,
};
use gdnative::{api::resource_loader, prelude::*};

#[derive(NativeClass)]
#[inherit(Node)]
pub struct X11_FG;
// Function that registers all exposed classes to Godot
fn init(handle: InitHandle) {
    handle.add_class::<X11_FG>();
}

impl X11_FG {
    /// The "constructor" of the class.
    fn new(_owner: &Node) -> Self {
        X11_FG
    }
}

#[methods]
impl X11_FG {
    #[export]
    fn _ready(&self, _owner: &Node) {
        godot_print!("Hello,X11!");
        return;
    }

    #[export]
    fn set_transient_for(&self, _owner: &Node, burrito_id: i32, gw2_id: i32) -> bool{
        godot_print!("Setting transient for");
        let mut dpy = DisplayConnection::create(None, None).unwrap();
        // let gw2 = get_window_from_name(&mut dpy, String::from("Guild Wars 2"));
        // if let None = gw2 {
        //     godot_print!("Failed to get gw2 window");
        //     return;
        // }
        //godot_print!("{:?}", gw2);
        let burrito = Window::const_from_xid(burrito_id as u32);
        let gw2 = Window::const_from_xid(gw2_id as u32);
        let res = burrito
            .change_property(
                &mut dpy,
                ATOM_WM_TRANSIENT_FOR,
                PropertyType::Window,
                PropertyFormat::ThirtyTwo,
                PropMode::Replace,
                &[gw2],
            );
        if let Ok(()) = res
        {
            godot_print!("Successfully set transient property!");
            return true;
        }
        godot_print!("Setting transient property failed");
        return false;
    }
}

fn get_window_from_name<D: Display>(dpy: &mut D, name: String) -> Option<Window> {
    let root = dpy.default_root();
    //TODO: Thread, that tries acquiring the window handle repeatedly until successful or terminated
    return search_window_from_name(dpy, &name, root);
}

fn search_window_from_name<D: Display>(
    dpy: &mut D,
    name: &String,
    current: Window,
) -> Option<Window> {
    let tree = current.query_tree_immediate(dpy).unwrap();
    let cur_name: String = current
        .get_property_immediate(dpy, ATOM_WM_NAME, PropertyType::String, false)
        .unwrap()
        .unwrap();
    if cur_name == *name {
        return Some(current);
    }
    //only need to look at children since we start at root
    for child in tree.children.iter() {
        if let Some(window) = search_window_from_name(dpy, &name, *child) {
            return Some(window);
        }
    }
    None
}

// Macro that creates the entry-points of the dynamic library.
godot_init!(init);

#[test]
fn test_search() {
    let mut dpy = DisplayConnection::create(None, None).unwrap();
    let _gw2 = get_window_from_name(&mut dpy, String::from("Guild Wars 2"));
    println!("{:?}", _gw2)
}
