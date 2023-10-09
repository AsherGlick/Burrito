#include "trail_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>

#include "attribute/bool.hpp"
#include "attribute/float.hpp"
#include "attribute/int.hpp"
#include "attribute/string.hpp"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

string Trail::classname() {
    return "Trail";
}

bool Trail::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, string base_dir) {
    string attributename;
    attributename = normalize(get_attribute_name(attribute));
    if (attributename == "achievementbit") {
        this->achievement_bitmask = parse_int(attribute, errors);
        this->achievement_bitmask_is_set = true;
    }
    else if (attributename == "achievementid") {
        this->achievement_id = parse_int(attribute, errors);
        this->achievement_id_is_set = true;
    }
    else if (attributename == "animspeed") {
        this->animation_speed = parse_float(attribute, errors);
        this->animation_speed_is_set = true;
    }
    else if (attributename == "animationspeed") {
        this->animation_speed = parse_float(attribute, errors);
        this->animation_speed_is_set = true;
    }
    else if (attributename == "canfade") {
        this->can_fade = parse_bool(attribute, errors);
        this->can_fade_is_set = true;
    }
    else if (attributename == "type") {
        this->category = parse_marker_category(attribute, errors);
        this->category_is_set = true;
    }
    else if (attributename == "category") {
        this->category = parse_marker_category(attribute, errors);
        this->category_is_set = true;
    }
    else if (attributename == "color") {
        this->color = parse_color(attribute, errors);
        this->color_is_set = true;
    }
    else if (attributename == "bhcolor") {
        this->color = parse_color(attribute, errors);
        this->color_is_set = true;
    }
    else if (attributename == "alpha") {
        this->color.alpha = parse_float(attribute, errors);
        this->color_is_set = true;
    }
    else if (attributename == "blue") {
        this->color.blue = parse_float(attribute, errors);
        this->color_is_set = true;
    }
    else if (attributename == "green") {
        this->color.green = parse_float(attribute, errors);
        this->color_is_set = true;
    }
    else if (attributename == "red") {
        this->color.red = parse_float(attribute, errors);
        this->color_is_set = true;
    }
    else if (attributename == "cull") {
        this->cull_chirality = parse_cull_chirality(attribute, errors);
        this->cull_chirality_is_set = true;
    }
    else if (attributename == "fadefar") {
        this->distance_fade_end = parse_float(attribute, errors);
        this->distance_fade_end_is_set = true;
    }
    else if (attributename == "distancefadeend") {
        this->distance_fade_end = parse_float(attribute, errors);
        this->distance_fade_end_is_set = true;
    }
    else if (attributename == "fadenear") {
        this->distance_fade_start = parse_float(attribute, errors);
        this->distance_fade_start_is_set = true;
    }
    else if (attributename == "distancefadestart") {
        this->distance_fade_start = parse_float(attribute, errors);
        this->distance_fade_start_is_set = true;
    }
    else if (attributename == "festival") {
        this->festival_filter = parse_festival_filter(attribute, errors);
        this->festival_filter_is_set = true;
    }
    else if (attributename == "guid") {
        this->guid = parse_unique_id(attribute, errors);
        this->guid_is_set = true;
    }
    else if (attributename == "iswall") {
        this->is_wall = parse_bool(attribute, errors);
        this->is_wall_is_set = true;
    }
    else if (attributename == "mapdisplaysize") {
        this->map_display_size = parse_int(attribute, errors);
        this->map_display_size_is_set = true;
    }
    else if (attributename == "mapid") {
        this->map_id = parse_int(attribute, errors);
        this->map_id_is_set = true;
    }
    else if (attributename == "maptype") {
        this->map_type_filter = parse_map_type_filter(attribute, errors);
        this->map_type_filter_is_set = true;
    }
    else if (attributename == "mount") {
        this->mount_filter = parse_mount_filter(attribute, errors);
        this->mount_filter_is_set = true;
    }
    else if (attributename == "profession") {
        this->profession_filter = parse_profession_filter(attribute, errors);
        this->profession_filter_is_set = true;
    }
    else if (attributename == "ingamevisibility") {
        this->render_ingame = parse_bool(attribute, errors);
        this->render_ingame_is_set = true;
    }
    else if (attributename == "bhingamevisibility") {
        this->render_ingame = parse_bool(attribute, errors);
        this->render_ingame_is_set = true;
    }
    else if (attributename == "mapvisibility") {
        this->render_on_map = parse_bool(attribute, errors);
        this->render_on_map_is_set = true;
    }
    else if (attributename == "bhmapvisibility") {
        this->render_on_map = parse_bool(attribute, errors);
        this->render_on_map_is_set = true;
    }
    else if (attributename == "minimapvisibility") {
        this->render_on_minimap = parse_bool(attribute, errors);
        this->render_on_minimap_is_set = true;
    }
    else if (attributename == "bhminimapvisibility") {
        this->render_on_minimap = parse_bool(attribute, errors);
        this->render_on_minimap_is_set = true;
    }
    else if (attributename == "schedule") {
        this->schedule = parse_string(attribute, errors);
        this->schedule_is_set = true;
    }
    else if (attributename == "scheduleduration") {
        this->schedule_duration = parse_float(attribute, errors);
        this->schedule_duration_is_set = true;
    }
    else if (attributename == "specialization") {
        this->specialization_filter = parse_specialization_filter(attribute, errors);
        this->specialization_filter_is_set = true;
    }
    else if (attributename == "race") {
        this->species_filter = parse_species_filter(attribute, errors);
        this->species_filter_is_set = true;
    }
    else if (attributename == "species") {
        this->species_filter = parse_species_filter(attribute, errors);
        this->species_filter_is_set = true;
    }
    else if (attributename == "texture") {
        this->texture = parse_image(attribute, errors);
        this->texture_is_set = true;
    }
    else if (attributename == "traildata") {
        this->trail_data = parse_trail_data(attribute, errors, base_dir);
        this->trail_data_is_set = true;
        this->map_id = this->trail_data.side_effect_map_id;
        this->map_id_is_set = true;
    }
    else if (attributename == "trailscale") {
        this->trail_scale = parse_float(attribute, errors);
        this->trail_scale_is_set = true;
    }
    else {
        return false;
    }
    return true;
}

bool Trail::validate_attributes_of_type_marker_category() {
    // TODO: validate "category"
    return true;
}

vector<string> Trail::as_xml() const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<Trail ");
    if (this->achievement_bitmask_is_set) {
        xml_node_contents.push_back(" AchievementBit=\"" + stringify_int(this->achievement_bitmask) + "\"");
    }
    if (this->achievement_id_is_set) {
        xml_node_contents.push_back(" AchievementId=\"" + stringify_int(this->achievement_id) + "\"");
    }
    if (this->animation_speed_is_set) {
        xml_node_contents.push_back(" AnimSpeed=\"" + stringify_float(this->animation_speed) + "\"");
    }
    if (this->can_fade_is_set) {
        xml_node_contents.push_back(" CanFade=\"" + stringify_bool(this->can_fade) + "\"");
    }
    if (this->category_is_set) {
        xml_node_contents.push_back(" Type=\"" + stringify_marker_category(this->category) + "\"");
    }
    if (this->color_is_set) {
        xml_node_contents.push_back(" Color=\"" + stringify_color(this->color) + "\"");
    }
    if (this->color_is_set) {
        xml_node_contents.push_back(" Alpha=\"" + stringify_float(this->color.alpha) + "\"");
    }
    if (this->cull_chirality_is_set) {
        xml_node_contents.push_back(" Cull=\"" + stringify_cull_chirality(this->cull_chirality) + "\"");
    }
    if (this->distance_fade_end_is_set) {
        xml_node_contents.push_back(" FadeFar=\"" + stringify_float(this->distance_fade_end) + "\"");
    }
    if (this->distance_fade_start_is_set) {
        xml_node_contents.push_back(" FadeNear=\"" + stringify_float(this->distance_fade_start) + "\"");
    }
    if (this->festival_filter_is_set) {
        xml_node_contents.push_back(" Festival=\"" + stringify_festival_filter(this->festival_filter) + "\"");
    }
    if (this->guid_is_set) {
        xml_node_contents.push_back(" GUID=\"" + stringify_unique_id(this->guid) + "\"");
    }
    if (this->is_wall_is_set) {
        xml_node_contents.push_back(" IsWall=\"" + stringify_bool(this->is_wall) + "\"");
    }
    if (this->map_display_size_is_set) {
        xml_node_contents.push_back(" MapDisplaySize=\"" + stringify_int(this->map_display_size) + "\"");
    }
    if (this->map_id_is_set) {
        xml_node_contents.push_back(" MapID=\"" + stringify_int(this->map_id) + "\"");
    }
    if (this->map_type_filter_is_set) {
        xml_node_contents.push_back(" MapType=\"" + stringify_map_type_filter(this->map_type_filter) + "\"");
    }
    if (this->mount_filter_is_set) {
        xml_node_contents.push_back(" Mount=\"" + stringify_mount_filter(this->mount_filter) + "\"");
    }
    if (this->profession_filter_is_set) {
        xml_node_contents.push_back(" Profession=\"" + stringify_profession_filter(this->profession_filter) + "\"");
    }
    if (this->render_ingame_is_set) {
        xml_node_contents.push_back(" IngameVisibility=\"" + stringify_bool(this->render_ingame) + "\"");
    }
    if (this->render_on_map_is_set) {
        xml_node_contents.push_back(" MapVisibility=\"" + stringify_bool(this->render_on_map) + "\"");
    }
    if (this->render_on_minimap_is_set) {
        xml_node_contents.push_back(" MinimapVisibility=\"" + stringify_bool(this->render_on_minimap) + "\"");
    }
    if (this->schedule_is_set) {
        xml_node_contents.push_back(" Schedule=\"" + stringify_string(this->schedule) + "\"");
    }
    if (this->schedule_duration_is_set) {
        xml_node_contents.push_back(" ScheduleDuration=\"" + stringify_float(this->schedule_duration) + "\"");
    }
    if (this->specialization_filter_is_set) {
        xml_node_contents.push_back(" Specialization=\"" + stringify_specialization_filter(this->specialization_filter) + "\"");
    }
    if (this->species_filter_is_set) {
        xml_node_contents.push_back(" Race=\"" + stringify_species_filter(this->species_filter) + "\"");
    }
    if (this->texture_is_set) {
        xml_node_contents.push_back(" Texture=\"" + stringify_image(this->texture) + "\"");
    }
    if (this->trail_data_is_set) {
        xml_node_contents.push_back(" TrailData=\"" + stringify_trail_data(this->trail_data) + "\"");
    }
    if (this->trail_scale_is_set) {
        xml_node_contents.push_back(" TrailScale=\"" + stringify_float(this->trail_scale) + "\"");
    }
    xml_node_contents.push_back("/>");
    return xml_node_contents;
}

waypoint::Trail Trail::as_protobuf() const {
    waypoint::Trail proto_trail;
    if (this->achievement_bitmask_is_set) {
        proto_trail.set_achievement_bit(to_proto_int(this->achievement_bitmask));
    }
    if (this->achievement_id_is_set) {
        proto_trail.set_achievement_id(to_proto_int(this->achievement_id));
    }
    if (this->animation_speed_is_set) {
        proto_trail.set_animation_speed(to_proto_float(this->animation_speed));
    }
    if (this->can_fade_is_set) {
        proto_trail.set_can_fade(to_proto_bool(this->can_fade));
    }
    if (this->category_is_set) {
        proto_trail.set_allocated_category(to_proto_marker_category(this->category));
    }
    if (this->color_is_set) {
        proto_trail.set_allocated_rgba_color(to_proto_color(this->color));
    }
    if (this->cull_chirality_is_set) {
        proto_trail.set_cull_chirality(to_proto_cull_chirality(this->cull_chirality));
    }
    if (this->distance_fade_end_is_set) {
        proto_trail.set_distance_fade_end(to_proto_float(this->distance_fade_end));
    }
    if (this->distance_fade_start_is_set) {
        proto_trail.set_distance_fade_start(to_proto_float(this->distance_fade_start));
    }
    if (this->festival_filter_is_set) {
        proto_trail.set_allocated_festival_filter(to_proto_festival_filter(this->festival_filter));
    }
    if (this->guid_is_set) {
        proto_trail.set_allocated_guid(to_proto_unique_id(this->guid));
    }
    if (this->is_wall_is_set) {
        proto_trail.set_is_wall(to_proto_bool(this->is_wall));
    }
    if (this->map_display_size_is_set) {
        proto_trail.set_map_display_size(to_proto_int(this->map_display_size));
    }
    if (this->map_id_is_set) {
        proto_trail.set_map_id(to_proto_int(this->map_id));
    }
    if (this->map_type_filter_is_set) {
        proto_trail.set_allocated_map_type_filter(to_proto_map_type_filter(this->map_type_filter));
    }
    if (this->mount_filter_is_set) {
        proto_trail.set_allocated_mount_filter(to_proto_mount_filter(this->mount_filter));
    }
    if (this->profession_filter_is_set) {
        proto_trail.set_allocated_profession_filter(to_proto_profession_filter(this->profession_filter));
    }
    if (this->render_ingame_is_set) {
        proto_trail.set_tentative__render_ingame(to_proto_bool(this->render_ingame));
    }
    if (this->render_on_map_is_set) {
        proto_trail.set_tentative__render_on_map(to_proto_bool(this->render_on_map));
    }
    if (this->render_on_minimap_is_set) {
        proto_trail.set_tentative__render_on_minimap(to_proto_bool(this->render_on_minimap));
    }
    if (this->schedule_is_set) {
        proto_trail.set_bhdraft__schedule(to_proto_string(this->schedule));
    }
    if (this->schedule_duration_is_set) {
        proto_trail.set_bhdraft__schedule_duration(to_proto_float(this->schedule_duration));
    }
    if (this->specialization_filter_is_set) {
        proto_trail.set_allocated_specialization_filter(to_proto_specialization_filter(this->specialization_filter));
    }
    if (this->species_filter_is_set) {
        proto_trail.set_allocated_species_filter(to_proto_species_filter(this->species_filter));
    }
    if (this->texture_is_set) {
        proto_trail.set_allocated_texture_path(to_proto_image(this->texture));
    }
    if (this->trail_data_is_set) {
        proto_trail.set_allocated_trail_data(to_proto_trail_data(this->trail_data));
    }
    if (this->trail_scale_is_set) {
        proto_trail.set_scale(to_proto_float(this->trail_scale));
    }
    return proto_trail;
}

void Trail::parse_protobuf(waypoint::Trail proto_trail) {
    if (proto_trail.achievement_bit() != 0) {
        this->achievement_bitmask = from_proto_int(proto_trail.achievement_bit());
        this->achievement_bitmask_is_set = true;
    }
    if (proto_trail.achievement_id() != 0) {
        this->achievement_id = from_proto_int(proto_trail.achievement_id());
        this->achievement_id_is_set = true;
    }
    if (proto_trail.animation_speed() != 0) {
        this->animation_speed = from_proto_float(proto_trail.animation_speed());
        this->animation_speed_is_set = true;
    }
    if (proto_trail.can_fade() != 0) {
        this->can_fade = from_proto_bool(proto_trail.can_fade());
        this->can_fade_is_set = true;
    }
    if (proto_trail.has_category()) {
        this->category = from_proto_marker_category(proto_trail.category());
        this->category_is_set = true;
    }
    if (proto_trail.has_rgba_color()) {
        this->color = from_proto_color(proto_trail.rgba_color());
        this->color_is_set = true;
    }
    if (proto_trail.cull_chirality() != 0) {
        this->cull_chirality = from_proto_cull_chirality(proto_trail.cull_chirality());
        this->cull_chirality_is_set = true;
    }
    if (proto_trail.distance_fade_end() != 0) {
        this->distance_fade_end = from_proto_float(proto_trail.distance_fade_end());
        this->distance_fade_end_is_set = true;
    }
    if (proto_trail.distance_fade_start() != 0) {
        this->distance_fade_start = from_proto_float(proto_trail.distance_fade_start());
        this->distance_fade_start_is_set = true;
    }
    if (proto_trail.has_festival_filter()) {
        this->festival_filter = from_proto_festival_filter(proto_trail.festival_filter());
        this->festival_filter_is_set = true;
    }
    if (proto_trail.has_guid()) {
        this->guid = from_proto_unique_id(proto_trail.guid());
        this->guid_is_set = true;
    }
    if (proto_trail.is_wall() != 0) {
        this->is_wall = from_proto_bool(proto_trail.is_wall());
        this->is_wall_is_set = true;
    }
    if (proto_trail.map_display_size() != 0) {
        this->map_display_size = from_proto_int(proto_trail.map_display_size());
        this->map_display_size_is_set = true;
    }
    if (proto_trail.map_id() != 0) {
        this->map_id = from_proto_int(proto_trail.map_id());
        this->map_id_is_set = true;
    }
    if (proto_trail.has_map_type_filter()) {
        this->map_type_filter = from_proto_map_type_filter(proto_trail.map_type_filter());
        this->map_type_filter_is_set = true;
    }
    if (proto_trail.has_mount_filter()) {
        this->mount_filter = from_proto_mount_filter(proto_trail.mount_filter());
        this->mount_filter_is_set = true;
    }
    if (proto_trail.has_profession_filter()) {
        this->profession_filter = from_proto_profession_filter(proto_trail.profession_filter());
        this->profession_filter_is_set = true;
    }
    if (proto_trail.tentative__render_ingame() != 0) {
        this->render_ingame = from_proto_bool(proto_trail.tentative__render_ingame());
        this->render_ingame_is_set = true;
    }
    if (proto_trail.tentative__render_on_map() != 0) {
        this->render_on_map = from_proto_bool(proto_trail.tentative__render_on_map());
        this->render_on_map_is_set = true;
    }
    if (proto_trail.tentative__render_on_minimap() != 0) {
        this->render_on_minimap = from_proto_bool(proto_trail.tentative__render_on_minimap());
        this->render_on_minimap_is_set = true;
    }
    if (proto_trail.bhdraft__schedule() != "") {
        this->schedule = from_proto_string(proto_trail.bhdraft__schedule());
        this->schedule_is_set = true;
    }
    if (proto_trail.bhdraft__schedule_duration() != 0) {
        this->schedule_duration = from_proto_float(proto_trail.bhdraft__schedule_duration());
        this->schedule_duration_is_set = true;
    }
    if (proto_trail.has_specialization_filter()) {
        this->specialization_filter = from_proto_specialization_filter(proto_trail.specialization_filter());
        this->specialization_filter_is_set = true;
    }
    if (proto_trail.has_species_filter()) {
        this->species_filter = from_proto_species_filter(proto_trail.species_filter());
        this->species_filter_is_set = true;
    }
    if (proto_trail.has_texture_path()) {
        this->texture = from_proto_image(proto_trail.texture_path());
        this->texture_is_set = true;
    }
    if (proto_trail.has_trail_data()) {
        this->trail_data = from_proto_trail_data(proto_trail.trail_data());
        this->trail_data_is_set = true;
    }
    if (proto_trail.scale() != 0) {
        this->trail_scale = from_proto_float(proto_trail.scale());
        this->trail_scale_is_set = true;
    }
}
