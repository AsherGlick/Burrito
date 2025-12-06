use breadx::{
    auto::xproto::ATOM_WM_TRANSIENT_FOR,
    traits::{PropMode, PropertyFormat, PropertyType, ATOM_WM_NAME},
    Display, DisplayBase, DisplayConnection, Window,
};
use gdnative::prelude::*;
use std::os::raw::c_int;

#[link(name = "X11")]
#[link(name = "Xext")]
extern "C" {
    pub fn XOpenDisplay(name: *const i8) -> *mut XDisplay;
    pub fn XPolygonRegion(
        points: *const XPoint,
        npoints: i32,
        fill_rule: i32,
    ) -> XRegion;
    pub fn XShapeCombineRegion(
        dpy: *mut XDisplay,
        dest: XWindow,
        dest_kind: i32,
        xoff: i32,
        yoff: i32,
        region: XRegion,
        op: i32,
    );
    pub fn XDestroyRegion(region: XRegion);
    pub fn XFlush(dpy: *mut XDisplay) -> i32;
}

pub enum XDisplay {}
pub type XWindow = u64;
pub type XRegion = *mut std::ffi::c_void;

#[repr(C)]
pub struct XPoint {
    pub x: i16,
    pub y: i16,
}

#[repr(C)]
#[derive(ToVariant, FromVariant)]
pub struct GdRect {
    pub x: i32,
    pub y: i32,
    pub w: i32,
    pub h: i32,
}

#[allow(non_upper_case_globals)]
const ShapeInput: c_int = 2;
#[allow(non_upper_case_globals)]
const ShapeSet: c_int = 0;
#[allow(non_upper_case_globals)]
const ShapeUnion: c_int = 1;

#[derive(NativeClass)]
#[inherit(Node)]
pub struct X11_FG {
    dpy: *mut XDisplay,
}
// Function that registers all exposed classes to Godot
fn init(handle: InitHandle) {
    handle.add_class::<X11_FG>();
}

impl X11_FG {
    /// The "constructor" of the class.
    fn new(_owner: &Node) -> Self {
        let dpy = unsafe { XOpenDisplay(std::ptr::null()) };
        X11_FG { dpy }
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
    fn set_input_shapes(&self, _node: &Node, burrito_id: i32, rects: Vec<GdRect>) -> bool {
        let window = burrito_id as XWindow;
        if self.dpy.is_null() {
            return false;
        }

        unsafe {
            for (i, r) in rects.iter().enumerate() {
                let pts = [
                    XPoint { x: r.x         as i16,  y: r.y         as i16 },
                    XPoint { x: (r.x + r.w) as i16,  y: r.y         as i16 },
                    XPoint { x: (r.x + r.w) as i16,  y: (r.y + r.h) as i16 },
                    XPoint { x: r.x         as i16,  y: (r.y + r.h) as i16 },
                ];

                let region = XPolygonRegion(pts.as_ptr(), pts.len() as c_int, 0);
                let op = if i == 0 { ShapeSet } else { ShapeUnion };

                XShapeCombineRegion(
                    self.dpy,
                    window,
                    ShapeInput,
                    0,
                    0,
                    region,
                    op,
                );

                XDestroyRegion(region);
                godot_print!("set_input_shapes called ({},{}) -> ({}, {})", pts[0].x, pts[0].y, pts[3].x, pts[3].y);
            }
            godot_print!("");
            XFlush(self.dpy);
        }
        true
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

    #[export]
    fn get_window_geometry(&self, _owner: &Node, gw2_id: i32) -> (u16, u16){
        let mut dpy = DisplayConnection::create(None, None).unwrap();
        let window = Window::const_from_xid(gw2_id as u32);
        let geometry = window.geometry_immediate(&mut dpy).unwrap();
        return (geometry.width, geometry.height)
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
