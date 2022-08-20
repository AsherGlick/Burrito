#include "trail.hpp"

using namespace std;

string Trail::classname() {
    return "Trail";
}
bool Trail::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors) {
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
    else if (attributename == "animspeed") {
        this->animation_speed = parse_float(attribute, errors);
    }
    else if (attributename == "animationspeed") {
        this->animation_speed = parse_float(attribute, errors);
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
    else if (attributename == "festival") {
        this->festival_filter = parse_festival_filter(attribute, errors);
    }
    else if (attributename == "guid") {
        this->guid = parse_unique_id(attribute, errors);
    }
    else if (attributename == "iswall") {
        this->is_wall = parse_bool(attribute, errors);
    }
    else if (attributename == "mapid") {
        this->map_id = parse_int(attribute, errors);
    }
    else if (attributename == "mapid") {
        this->map_type_filter = parse_map_type_filter(attribute, errors);
    }
    else if (attributename == "mount") {
        this->mount_filter = parse_mount_filter(attribute, errors);
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
    else if (attributename == "schedule") {
        this->schedule = parse_string(attribute, errors);
    }
    else if (attributename == "scheduleduration") {
        this->schedule_duration = parse_float(attribute, errors);
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
    else if (attributename == "texture") {
        this->texture = parse_image(attribute, errors);
    }
    else if (attributename == "traildata") {
        this->trail_data = parse_trail_data(attribute, errors);
    }
    else if (attributename == "traildata") {
        this->trail_data_map_id = parse_trail_data_map_id(attribute, errors);
    }
    else if (attributename == "trailscale") {
        this->trail_scale = parse_float(attribute, errors);
    }
    else {
        return false;
    }
    return true;
}