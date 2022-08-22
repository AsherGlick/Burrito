#include "icon_gen.hpp"

using namespace std;

string Icon::classname() {
    return "POI";
}
bool Icon::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors) {
    string attributename; 
    attributename = normalize_type_name(get_attribute_name(attribute)); 
    if (attributename == "achievementbit") {
        this->achievement_bitmask = parse_int(attribute, errors);
    }
    else if (attributename == "achievementid") {
        this->achievement_id = parse_int(attribute, errors);
    }
    else if (attributename == "alpha") {
        this->alpha = parse_float(attribute, errors);
    }
    else if (attributename == "autotrigger") {
        this->auto_trigger = parse_bool(attribute, errors);
    }
    else if (attributename == "bouncedelay") {
        this->bounce_delay = parse_float(attribute, errors);
    }
    else if (attributename == "bounceduration") {
        this->bounce_duration = parse_float(attribute, errors);
    }
    else if (attributename == "bounceheight") {
        this->bounce_height = parse_float(attribute, errors);
    }
    else if (attributename == "canfade") {
        this->can_fade = parse_bool(attribute, errors);
    }
    else if (attributename == "type") {
        this->category = parse_marker_category(attribute, errors);
    }
    else if (attributename == "category") {
        this->category = parse_marker_category(attribute, errors);
    }
    else if (attributename == "color") {
        this->color = parse_color(attribute, errors);
    }
    else if (attributename == "copy") {
        this->copy_clipboard = parse_string(attribute, errors);
    }
    else if (attributename == "copyclipboard") {
        this->copy_clipboard = parse_string(attribute, errors);
    }
    else if (attributename == "copymessage") {
        this->copy_message = parse_string(attribute, errors);
    }
    else if (attributename == "cull") {
        this->cull_chirality = parse_cull_chirality(attribute, errors);
    }
    else if (attributename == "fadefar") {
        this->distance_fade_end = parse_float(attribute, errors);
    }
    else if (attributename == "distancefadeend") {
        this->distance_fade_end = parse_float(attribute, errors);
    }
    else if (attributename == "fadenear") {
        this->distance_fade_start = parse_float(attribute, errors);
    }
    else if (attributename == "distancefadestart") {
        this->distance_fade_start = parse_float(attribute, errors);
    }
    else if (attributename == "rotation") {
        this->euler_rotation = parse_euler_rotation(attribute, errors);
    }
    else if (attributename == "rotatex") {
        this->euler_rotation = parse_euler_rotation(attribute, errors);
    }
    else if (attributename == "rotatey") {
        this->euler_rotation = parse_euler_rotation(attribute, errors);
    }
    else if (attributename == "rotatez") {
        this->euler_rotation = parse_euler_rotation(attribute, errors);
    }
    else if (attributename == "festival") {
        this->festival_filter = parse_festival_filter(attribute, errors);
    }
    else if (attributename == "guid") {
        this->guid = parse_unique_id(attribute, errors);
    }
    else if (attributename == "hascountdown") {
        this->has_countdown = parse_bool(attribute, errors);
    }
    else if (attributename == "heightoffset") {
        this->heightoffset = parse_float(attribute, errors);
    }
    else if (attributename == "hide") {
        this->hide_category = parse_marker_category(attribute, errors);
    }
    else if (attributename == "iconfile") {
        this->icon = parse_image(attribute, errors);
    }
    else if (attributename == "iconsize") {
        this->icon_size = parse_float(attribute, errors);
    }
    else if (attributename == "info") {
        this->info_message = parse_string(attribute, errors);
    }
    else if (attributename == "invertbehavior") {
        this->invert_visibility = parse_bool(attribute, errors);
    }
    else if (attributename == "mapdisplaysize") {
        this->map_display_size = parse_int(attribute, errors);
    }
    else if (attributename == "mapid") {
        this->map_id = parse_int(attribute, errors);
    }
    else if (attributename == "mapid") {
        this->map_type_filter = parse_map_type_filter(attribute, errors);
    }
    else if (attributename == "maxsize") {
        this->maximum_size_on_screen = parse_int(attribute, errors);
    }
    else if (attributename == "minsize") {
        this->minimum_size_on_screen = parse_int(attribute, errors);
    }
    else if (attributename == "mount") {
        this->mount_filter = parse_mount_filter(attribute, errors);
    }
    else if (attributename == "position") {
        this->position = parse_position(attribute, errors);
    }
    else if (attributename == "xpos") {
        this->position = parse_position(attribute, errors);
    }
    else if (attributename == "positionx") {
        this->position = parse_position(attribute, errors);
    }
    else if (attributename == "ypos") {
        this->position = parse_position(attribute, errors);
    }
    else if (attributename == "positiony") {
        this->position = parse_position(attribute, errors);
    }
    else if (attributename == "zpos") {
        this->position = parse_position(attribute, errors);
    }
    else if (attributename == "positionz") {
        this->position = parse_position(attribute, errors);
    }
    else if (attributename == "profession") {
        this->profession_filter = parse_profession_filter(attribute, errors);
    }
    else if (attributename == "ingamevisibility") {
        this->render_ingame = parse_bool(attribute, errors);
    }
    else if (attributename == "bhingamevisibility") {
        this->render_ingame = parse_bool(attribute, errors);
    }
    else if (attributename == "mapvisibility") {
        this->render_on_map = parse_bool(attribute, errors);
    }
    else if (attributename == "bhmapvisibility") {
        this->render_on_map = parse_bool(attribute, errors);
    }
    else if (attributename == "minimapvisibility") {
        this->render_on_minimap = parse_bool(attribute, errors);
    }
    else if (attributename == "bhminimapvisibility") {
        this->render_on_minimap = parse_bool(attribute, errors);
    }
    else if (attributename == "behavior") {
        this->reset_behavior = parse_reset_behavior(attribute, errors);
    }
    else if (attributename == "resetlength") {
        this->reset_length = parse_float(attribute, errors);
    }
    else if (attributename == "scaleonmapwithzoom") {
        this->scale_on_map_with_zoom = parse_bool(attribute, errors);
    }
    else if (attributename == "schedule") {
        this->schedule = parse_string(attribute, errors);
    }
    else if (attributename == "scheduleduration") {
        this->schedule_duration = parse_float(attribute, errors);
    }
    else if (attributename == "show") {
        this->show_category = parse_marker_category(attribute, errors);
    }
    else if (attributename == "specialization") {
        this->specialization_filter = parse_specialization_filter(attribute, errors);
    }
    else if (attributename == "race") {
        this->species_filter = parse_species_filter(attribute, errors);
    }
    else if (attributename == "species") {
        this->species_filter = parse_species_filter(attribute, errors);
    }
    else if (attributename == "toggle") {
        this->toggle_category = parse_marker_category(attribute, errors);
    }
    else if (attributename == "tipname") {
        this->tooltip_description = parse_string(attribute, errors);
    }
    else if (attributename == "tipdescription") {
        this->tooltip_name = parse_string(attribute, errors);
    }
    else if (attributename == "triggerrange") {
        this->trigger_range = parse_float(attribute, errors);
    }
    else if (attributename == "inforange") {
        this->trigger_range = parse_float(attribute, errors);
    }
    else {
        return false;
    }
    return true;
}
bool Icon::validate_attributes_of_type_marker_category(){
    // TODO: validate "show_category"
    // TODO: validate "hide_category"
    // TODO: validate "toggle_category"
    // TODO: validate "category"
    return true;
}