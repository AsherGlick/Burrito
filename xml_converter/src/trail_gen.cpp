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
        xml_attribute_to_int(attribute, errors, &(this->achievement_bitmask), &(this->achievement_bitmask_is_set));
    }
    else if (attributename == "achievementid") {
        xml_attribute_to_int(attribute, errors, &(this->achievement_id), &(this->achievement_id_is_set));
    }
    else if (attributename == "animspeed") {
        xml_attribute_to_float(attribute, errors, &(this->animation_speed), &(this->animation_speed_is_set));
    }
    else if (attributename == "animationspeed") {
        xml_attribute_to_float(attribute, errors, &(this->animation_speed), &(this->animation_speed_is_set));
    }
    else if (attributename == "canfade") {
        xml_attribute_to_bool(attribute, errors, &(this->can_fade), &(this->can_fade_is_set));
    }
    else if (attributename == "type") {
        xml_attribute_to_marker_category(attribute, errors, &(this->category), &(this->category_is_set));
    }
    else if (attributename == "category") {
        xml_attribute_to_marker_category(attribute, errors, &(this->category), &(this->category_is_set));
    }
    else if (attributename == "color") {
        xml_attribute_to_color(attribute, errors, &(this->color), &(this->color_is_set));
    }
    else if (attributename == "bhcolor") {
        xml_attribute_to_color(attribute, errors, &(this->color), &(this->color_is_set));
    }
    else if (attributename == "alpha") {
        xml_attribute_to_float(attribute, errors, &(this->color.alpha), &(this->color_is_set));
    }
    else if (attributename == "blue") {
        xml_attribute_to_float(attribute, errors, &(this->color.blue), &(this->color_is_set));
    }
    else if (attributename == "green") {
        xml_attribute_to_float(attribute, errors, &(this->color.green), &(this->color_is_set));
    }
    else if (attributename == "red") {
        xml_attribute_to_float(attribute, errors, &(this->color.red), &(this->color_is_set));
    }
    else if (attributename == "cull") {
        xml_attribute_to_cull_chirality(attribute, errors, &(this->cull_chirality), &(this->cull_chirality_is_set));
    }
    else if (attributename == "fadefar") {
        xml_attribute_to_float(attribute, errors, &(this->distance_fade_end), &(this->distance_fade_end_is_set));
    }
    else if (attributename == "distancefadeend") {
        xml_attribute_to_float(attribute, errors, &(this->distance_fade_end), &(this->distance_fade_end_is_set));
    }
    else if (attributename == "fadenear") {
        xml_attribute_to_float(attribute, errors, &(this->distance_fade_start), &(this->distance_fade_start_is_set));
    }
    else if (attributename == "distancefadestart") {
        xml_attribute_to_float(attribute, errors, &(this->distance_fade_start), &(this->distance_fade_start_is_set));
    }
    else if (attributename == "festival") {
        xml_attribute_to_festival_filter(attribute, errors, &(this->festival_filter), &(this->festival_filter_is_set));
    }
    else if (attributename == "guid") {
        xml_attribute_to_unique_id(attribute, errors, &(this->guid), &(this->guid_is_set));
    }
    else if (attributename == "iswall") {
        xml_attribute_to_bool(attribute, errors, &(this->is_wall), &(this->is_wall_is_set));
    }
    else if (attributename == "mapdisplaysize") {
        xml_attribute_to_int(attribute, errors, &(this->map_display_size), &(this->map_display_size_is_set));
    }
    else if (attributename == "mapid") {
        xml_attribute_to_int(attribute, errors, &(this->map_id), &(this->map_id_is_set));
    }
    else if (attributename == "maptype") {
        xml_attribute_to_map_type_filter(attribute, errors, &(this->map_type_filter), &(this->map_type_filter_is_set));
    }
    else if (attributename == "mount") {
        xml_attribute_to_mount_filter(attribute, errors, &(this->mount_filter), &(this->mount_filter_is_set));
    }
    else if (attributename == "profession") {
        xml_attribute_to_profession_filter(attribute, errors, &(this->profession_filter), &(this->profession_filter_is_set));
    }
    else if (attributename == "ingamevisibility") {
        xml_attribute_to_bool(attribute, errors, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    else if (attributename == "bhingamevisibility") {
        xml_attribute_to_bool(attribute, errors, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    else if (attributename == "mapvisibility") {
        xml_attribute_to_bool(attribute, errors, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    else if (attributename == "bhmapvisibility") {
        xml_attribute_to_bool(attribute, errors, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    else if (attributename == "minimapvisibility") {
        xml_attribute_to_bool(attribute, errors, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    else if (attributename == "bhminimapvisibility") {
        xml_attribute_to_bool(attribute, errors, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    else if (attributename == "schedule") {
        xml_attribute_to_string(attribute, errors, &(this->schedule), &(this->schedule_is_set));
    }
    else if (attributename == "scheduleduration") {
        xml_attribute_to_float(attribute, errors, &(this->schedule_duration), &(this->schedule_duration_is_set));
    }
    else if (attributename == "specialization") {
        xml_attribute_to_specialization_filter(attribute, errors, &(this->specialization_filter), &(this->specialization_filter_is_set));
    }
    else if (attributename == "race") {
        xml_attribute_to_species_filter(attribute, errors, &(this->species_filter), &(this->species_filter_is_set));
    }
    else if (attributename == "species") {
        xml_attribute_to_species_filter(attribute, errors, &(this->species_filter), &(this->species_filter_is_set));
    }
    else if (attributename == "texture") {
        xml_attribute_to_image(attribute, errors, &(this->texture), &(this->texture_is_set));
    }
    else if (attributename == "traildata") {
        xml_attribute_to_trail_data(attribute, errors, base_dir, &(this->trail_data), &(this->trail_data_is_set), &(this->map_id), &(this->map_id_is_set));
    }
    else if (attributename == "trailscale") {
        xml_attribute_to_float(attribute, errors, &(this->trail_scale), &(this->trail_scale_is_set));
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
        xml_node_contents.push_back(int_to_xml_attribute("AchievementBit", &this->achievement_bitmask));
    }
    if (this->achievement_id_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("AchievementId", &this->achievement_id));
    }
    if (this->animation_speed_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("AnimSpeed", &this->animation_speed));
    }
    if (this->can_fade_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("CanFade", &this->can_fade));
    }
    if (this->category_is_set) {
        xml_node_contents.push_back(marker_category_to_xml_attribute("Type", &this->category));
    }
    if (this->color_is_set) {
        xml_node_contents.push_back(color_to_xml_attribute("Color", &this->color));
    }
    if (this->color_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("Alpha", &this->color.alpha));
    }
    if (this->cull_chirality_is_set) {
        xml_node_contents.push_back(cull_chirality_to_xml_attribute("Cull", &this->cull_chirality));
    }
    if (this->distance_fade_end_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("FadeFar", &this->distance_fade_end));
    }
    if (this->distance_fade_start_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("FadeNear", &this->distance_fade_start));
    }
    if (this->festival_filter_is_set) {
        xml_node_contents.push_back(festival_filter_to_xml_attribute("Festival", &this->festival_filter));
    }
    if (this->guid_is_set) {
        xml_node_contents.push_back(unique_id_to_xml_attribute("GUID", &this->guid));
    }
    if (this->is_wall_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("IsWall", &this->is_wall));
    }
    if (this->map_display_size_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("MapDisplaySize", &this->map_display_size));
    }
    if (this->map_id_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("MapID", &this->map_id));
    }
    if (this->map_type_filter_is_set) {
        xml_node_contents.push_back(map_type_filter_to_xml_attribute("MapType", &this->map_type_filter));
    }
    if (this->mount_filter_is_set) {
        xml_node_contents.push_back(mount_filter_to_xml_attribute("Mount", &this->mount_filter));
    }
    if (this->profession_filter_is_set) {
        xml_node_contents.push_back(profession_filter_to_xml_attribute("Profession", &this->profession_filter));
    }
    if (this->render_ingame_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("IngameVisibility", &this->render_ingame));
    }
    if (this->render_on_map_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("MapVisibility", &this->render_on_map));
    }
    if (this->render_on_minimap_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("MinimapVisibility", &this->render_on_minimap));
    }
    if (this->schedule_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("Schedule", &this->schedule));
    }
    if (this->schedule_duration_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("ScheduleDuration", &this->schedule_duration));
    }
    if (this->specialization_filter_is_set) {
        xml_node_contents.push_back(specialization_filter_to_xml_attribute("Specialization", &this->specialization_filter));
    }
    if (this->species_filter_is_set) {
        xml_node_contents.push_back(species_filter_to_xml_attribute("Race", &this->species_filter));
    }
    if (this->texture_is_set) {
        xml_node_contents.push_back(image_to_xml_attribute("Texture", &this->texture));
    }
    if (this->trail_data_is_set) {
        xml_node_contents.push_back(trail_data_to_xml_attribute("TrailData", &this->trail_data));
    }
    if (this->trail_scale_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("TrailScale", &this->trail_scale));
    }
    xml_node_contents.push_back("/>");
    return xml_node_contents;
}

waypoint::Trail Trail::as_protobuf() const {
    waypoint::Trail proto_trail;
    if (this->achievement_bitmask_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_achievement_bit(val); };
        int_to_proto(this->achievement_bitmask, setter);
    }
    if (this->achievement_id_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_achievement_id(val); };
        int_to_proto(this->achievement_id, setter);
    }
    if (this->animation_speed_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_animation_speed(val); };
        float_to_proto(this->animation_speed, setter);
    }
    if (this->can_fade_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_can_fade(val); };
        bool_to_proto(this->can_fade, setter);
    }
    if (this->category_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_category(val); };
        do_nothing(this->category, setter);
    }
    if (this->color_is_set) {
        std::function<void(waypoint::RGBAColor*)> setter = [&proto_trail](waypoint::RGBAColor* val) { proto_trail.set_allocated_rgba_color(val); };
        color_to_proto(this->color, setter);
    }
    if (this->cull_chirality_is_set) {
        std::function<void(waypoint::CullChirality)> setter = [&proto_trail](waypoint::CullChirality val) { proto_trail.set_cull_chirality(val); };
        cull_chirality_to_proto(this->cull_chirality, setter);
    }
    if (this->distance_fade_end_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_distance_fade_end(val); };
        float_to_proto(this->distance_fade_end, setter);
    }
    if (this->distance_fade_start_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_distance_fade_start(val); };
        float_to_proto(this->distance_fade_start, setter);
    }
    if (this->festival_filter_is_set) {
        std::function<void(waypoint::FestivalFilter*)> setter = [&proto_trail](waypoint::FestivalFilter* val) { proto_trail.set_allocated_festival_filter(val); };
        festival_filter_to_proto(this->festival_filter, setter);
    }
    if (this->guid_is_set) {
        std::function<void(std::string)> setter = [&proto_trail](std::string val) { proto_trail.set_guid(val); };
        unique_id_to_proto(this->guid, setter);
    }
    if (this->is_wall_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_is_wall(val); };
        bool_to_proto(this->is_wall, setter);
    }
    if (this->map_display_size_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_map_display_size(val); };
        int_to_proto(this->map_display_size, setter);
    }
    if (this->map_id_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_map_id(val); };
        int_to_proto(this->map_id, setter);
    }
    if (this->map_type_filter_is_set) {
        std::function<void(waypoint::MapTypeFilter*)> setter = [&proto_trail](waypoint::MapTypeFilter* val) { proto_trail.set_allocated_map_type_filter(val); };
        map_type_filter_to_proto(this->map_type_filter, setter);
    }
    if (this->mount_filter_is_set) {
        std::function<void(waypoint::MountFilter*)> setter = [&proto_trail](waypoint::MountFilter* val) { proto_trail.set_allocated_mount_filter(val); };
        mount_filter_to_proto(this->mount_filter, setter);
    }
    if (this->profession_filter_is_set) {
        std::function<void(waypoint::ProfessionFilter*)> setter = [&proto_trail](waypoint::ProfessionFilter* val) { proto_trail.set_allocated_profession_filter(val); };
        profession_filter_to_proto(this->profession_filter, setter);
    }
    if (this->render_ingame_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_tentative__render_ingame(val); };
        bool_to_proto(this->render_ingame, setter);
    }
    if (this->render_on_map_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_tentative__render_on_map(val); };
        bool_to_proto(this->render_on_map, setter);
    }
    if (this->render_on_minimap_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_tentative__render_on_minimap(val); };
        bool_to_proto(this->render_on_minimap, setter);
    }
    if (this->schedule_is_set) {
        std::function<void(std::string)> setter = [&proto_trail](std::string val) { proto_trail.set_bhdraft__schedule(val); };
        string_to_proto(this->schedule, setter);
    }
    if (this->schedule_duration_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_bhdraft__schedule_duration(val); };
        float_to_proto(this->schedule_duration, setter);
    }
    if (this->specialization_filter_is_set) {
        std::function<void(waypoint::SpecializationFilter*)> setter = [&proto_trail](waypoint::SpecializationFilter* val) { proto_trail.set_allocated_specialization_filter(val); };
        specialization_filter_to_proto(this->specialization_filter, setter);
    }
    if (this->species_filter_is_set) {
        std::function<void(waypoint::SpeciesFilter*)> setter = [&proto_trail](waypoint::SpeciesFilter* val) { proto_trail.set_allocated_species_filter(val); };
        species_filter_to_proto(this->species_filter, setter);
    }
    if (this->texture_is_set) {
        std::function<void(waypoint::TexturePath*)> setter = [&proto_trail](waypoint::TexturePath* val) { proto_trail.set_allocated_texture_path(val); };
        image_to_proto(this->texture, setter);
    }
    if (this->trail_data_is_set) {
        std::function<void(waypoint::TrailData*)> setter = [&proto_trail](waypoint::TrailData* val) { proto_trail.set_allocated_trail_data(val); };
        trail_data_to_proto(this->trail_data, setter);
    }
    if (this->trail_scale_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_scale(val); };
        float_to_proto(this->trail_scale, setter);
    }
    return proto_trail;
}

void Trail::parse_protobuf(waypoint::Trail proto_trail) {
    if (proto_trail.achievement_bit() != 0) {
        proto_to_int(proto_trail.achievement_bit(), &(this->achievement_bitmask), &(this->achievement_bitmask_is_set));
    }
    if (proto_trail.achievement_id() != 0) {
        proto_to_int(proto_trail.achievement_id(), &(this->achievement_id), &(this->achievement_id_is_set));
    }
    if (proto_trail.animation_speed() != 0) {
        proto_to_float(proto_trail.animation_speed(), &(this->animation_speed), &(this->animation_speed_is_set));
    }
    if (proto_trail.can_fade() != 0) {
        proto_to_bool(proto_trail.can_fade(), &(this->can_fade), &(this->can_fade_is_set));
    }
    if (proto_trail.category() != 0) {
        do_nothing(proto_trail.category(), &(this->category), &(this->category_is_set));
    }
    if (proto_trail.has_rgba_color()) {
        proto_to_color(proto_trail.rgba_color(), &(this->color), &(this->color_is_set));
    }
    if (proto_trail.cull_chirality() != 0) {
        proto_to_cull_chirality(proto_trail.cull_chirality(), &(this->cull_chirality), &(this->cull_chirality_is_set));
    }
    if (proto_trail.distance_fade_end() != 0) {
        proto_to_float(proto_trail.distance_fade_end(), &(this->distance_fade_end), &(this->distance_fade_end_is_set));
    }
    if (proto_trail.distance_fade_start() != 0) {
        proto_to_float(proto_trail.distance_fade_start(), &(this->distance_fade_start), &(this->distance_fade_start_is_set));
    }
    if (proto_trail.has_festival_filter()) {
        proto_to_festival_filter(proto_trail.festival_filter(), &(this->festival_filter), &(this->festival_filter_is_set));
    }
    if (proto_trail.guid() != "") {
        proto_to_unique_id(proto_trail.guid(), &(this->guid), &(this->guid_is_set));
    }
    if (proto_trail.is_wall() != 0) {
        proto_to_bool(proto_trail.is_wall(), &(this->is_wall), &(this->is_wall_is_set));
    }
    if (proto_trail.map_display_size() != 0) {
        proto_to_int(proto_trail.map_display_size(), &(this->map_display_size), &(this->map_display_size_is_set));
    }
    if (proto_trail.map_id() != 0) {
        proto_to_int(proto_trail.map_id(), &(this->map_id), &(this->map_id_is_set));
    }
    if (proto_trail.has_map_type_filter()) {
        proto_to_map_type_filter(proto_trail.map_type_filter(), &(this->map_type_filter), &(this->map_type_filter_is_set));
    }
    if (proto_trail.has_mount_filter()) {
        proto_to_mount_filter(proto_trail.mount_filter(), &(this->mount_filter), &(this->mount_filter_is_set));
    }
    if (proto_trail.has_profession_filter()) {
        proto_to_profession_filter(proto_trail.profession_filter(), &(this->profession_filter), &(this->profession_filter_is_set));
    }
    if (proto_trail.tentative__render_ingame() != 0) {
        proto_to_bool(proto_trail.tentative__render_ingame(), &(this->render_ingame), &(this->render_ingame_is_set));
    }
    if (proto_trail.tentative__render_on_map() != 0) {
        proto_to_bool(proto_trail.tentative__render_on_map(), &(this->render_on_map), &(this->render_on_map_is_set));
    }
    if (proto_trail.tentative__render_on_minimap() != 0) {
        proto_to_bool(proto_trail.tentative__render_on_minimap(), &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    if (proto_trail.bhdraft__schedule() != "") {
        proto_to_string(proto_trail.bhdraft__schedule(), &(this->schedule), &(this->schedule_is_set));
    }
    if (proto_trail.bhdraft__schedule_duration() != 0) {
        proto_to_float(proto_trail.bhdraft__schedule_duration(), &(this->schedule_duration), &(this->schedule_duration_is_set));
    }
    if (proto_trail.has_specialization_filter()) {
        proto_to_specialization_filter(proto_trail.specialization_filter(), &(this->specialization_filter), &(this->specialization_filter_is_set));
    }
    if (proto_trail.has_species_filter()) {
        proto_to_species_filter(proto_trail.species_filter(), &(this->species_filter), &(this->species_filter_is_set));
    }
    if (proto_trail.has_texture_path()) {
        proto_to_image(proto_trail.texture_path(), &(this->texture), &(this->texture_is_set));
    }
    if (proto_trail.has_trail_data()) {
        proto_to_trail_data(proto_trail.trail_data(), &(this->trail_data), &(this->trail_data_is_set));
    }
    if (proto_trail.scale() != 0) {
        proto_to_float(proto_trail.scale(), &(this->trail_scale), &(this->trail_scale_is_set));
    }
}
