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

bool Trail::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, XMLReaderState* state) {
    string attributename;
    attributename = normalize(get_attribute_name(attribute));
    if (attributename == "achievementbit") {
        xml_attribute_to_int(attribute, errors, state, &(this->achievement_bitmask), &(this->achievement_bitmask_is_set));
    }
    else if (attributename == "achievementid") {
        xml_attribute_to_int(attribute, errors, state, &(this->achievement_id), &(this->achievement_id_is_set));
    }
    else if (attributename == "animspeed") {
        xml_attribute_to_float(attribute, errors, state, &(this->animation_speed), &(this->animation_speed_is_set));
    }
    else if (attributename == "animationspeed") {
        xml_attribute_to_float(attribute, errors, state, &(this->animation_speed), &(this->animation_speed_is_set));
    }
    else if (attributename == "type") {
        xml_attribute_to_marker_category(attribute, errors, state, &(this->category), &(this->category_is_set));
    }
    else if (attributename == "category") {
        xml_attribute_to_marker_category(attribute, errors, state, &(this->category), &(this->category_is_set));
    }
    else if (attributename == "color") {
        xml_attribute_to_color(attribute, errors, state, &(this->color), &(this->color_is_set));
    }
    else if (attributename == "bhcolor") {
        xml_attribute_to_color(attribute, errors, state, &(this->color), &(this->color_is_set));
    }
    else if (attributename == "alpha") {
        xml_attribute_to_float(attribute, errors, state, &(this->color.alpha), &(this->color_is_set));
    }
    else if (attributename == "blue") {
        xml_attribute_to_float(attribute, errors, state, &(this->color.blue), &(this->color_is_set));
    }
    else if (attributename == "green") {
        xml_attribute_to_float(attribute, errors, state, &(this->color.green), &(this->color_is_set));
    }
    else if (attributename == "red") {
        xml_attribute_to_float(attribute, errors, state, &(this->color.red), &(this->color_is_set));
    }
    else if (attributename == "cull") {
        xml_attribute_to_cull_chirality(attribute, errors, state, &(this->cull_chirality), &(this->cull_chirality_is_set));
    }
    else if (attributename == "canfade") {
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->disable_player_cutout), &(this->disable_player_cutout_is_set));
    }
    else if (attributename == "fadefar") {
        xml_attribute_to_float(attribute, errors, state, &(this->distance_fade_end), &(this->distance_fade_end_is_set));
    }
    else if (attributename == "distancefadeend") {
        xml_attribute_to_float(attribute, errors, state, &(this->distance_fade_end), &(this->distance_fade_end_is_set));
    }
    else if (attributename == "fadenear") {
        xml_attribute_to_float(attribute, errors, state, &(this->distance_fade_start), &(this->distance_fade_start_is_set));
    }
    else if (attributename == "distancefadestart") {
        xml_attribute_to_float(attribute, errors, state, &(this->distance_fade_start), &(this->distance_fade_start_is_set));
    }
    else if (attributename == "festival") {
        xml_attribute_to_festival_filter(attribute, errors, state, &(this->festival_filter), &(this->festival_filter_is_set));
    }
    else if (attributename == "guid") {
        xml_attribute_to_unique_id(attribute, errors, state, &(this->guid), &(this->guid_is_set));
    }
    else if (attributename == "iswall") {
        xml_attribute_to_bool(attribute, errors, state, &(this->is_wall), &(this->is_wall_is_set));
    }
    else if (attributename == "mapdisplaysize") {
        xml_attribute_to_int(attribute, errors, state, &(this->map_display_size), &(this->map_display_size_is_set));
    }
    else if (attributename == "mapid") {
        xml_attribute_to_int(attribute, errors, state, &(this->map_id), &(this->map_id_is_set));
    }
    else if (attributename == "maptype") {
        xml_attribute_to_map_type_filter(attribute, errors, state, &(this->map_type_filter), &(this->map_type_filter_is_set));
    }
    else if (attributename == "mount") {
        xml_attribute_to_mount_filter(attribute, errors, state, &(this->mount_filter), &(this->mount_filter_is_set));
    }
    else if (attributename == "profession") {
        xml_attribute_to_profession_filter(attribute, errors, state, &(this->profession_filter), &(this->profession_filter_is_set));
    }
    else if (attributename == "ingamevisibility") {
        xml_attribute_to_bool(attribute, errors, state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    else if (attributename == "bhingamevisibility") {
        xml_attribute_to_bool(attribute, errors, state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    else if (attributename == "mapvisibility") {
        xml_attribute_to_bool(attribute, errors, state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    else if (attributename == "bhmapvisibility") {
        xml_attribute_to_bool(attribute, errors, state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    else if (attributename == "minimapvisibility") {
        xml_attribute_to_bool(attribute, errors, state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    else if (attributename == "bhminimapvisibility") {
        xml_attribute_to_bool(attribute, errors, state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    else if (attributename == "schedule") {
        xml_attribute_to_string(attribute, errors, state, &(this->schedule), &(this->schedule_is_set));
    }
    else if (attributename == "scheduleduration") {
        xml_attribute_to_float(attribute, errors, state, &(this->schedule_duration), &(this->schedule_duration_is_set));
    }
    else if (attributename == "specialization") {
        xml_attribute_to_specialization_filter(attribute, errors, state, &(this->specialization_filter), &(this->specialization_filter_is_set));
    }
    else if (attributename == "race") {
        xml_attribute_to_species_filter(attribute, errors, state, &(this->species_filter), &(this->species_filter_is_set));
    }
    else if (attributename == "species") {
        xml_attribute_to_species_filter(attribute, errors, state, &(this->species_filter), &(this->species_filter_is_set));
    }
    else if (attributename == "texture") {
        xml_attribute_to_image(attribute, errors, state, &(this->texture), &(this->texture_is_set));
    }
    else if (attributename == "traildata") {
        xml_attribute_to_trail_data(attribute, errors, state, &(this->trail_data), &(this->trail_data_is_set), &(this->map_id), &(this->map_id_is_set));
    }
    else if (attributename == "trailscale") {
        xml_attribute_to_float(attribute, errors, state, &(this->trail_scale), &(this->trail_scale_is_set));
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

vector<string> Trail::as_xml(XMLWriterState* state) const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<Trail ");
    if (this->achievement_bitmask_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("AchievementBit", state, &this->achievement_bitmask));
    }
    if (this->achievement_id_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("AchievementId", state, &this->achievement_id));
    }
    if (this->animation_speed_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("AnimSpeed", state, &this->animation_speed));
    }
    if (this->category_is_set) {
        xml_node_contents.push_back(marker_category_to_xml_attribute("Type", state, &this->category));
    }
    if (this->color_is_set) {
        xml_node_contents.push_back(color_to_xml_attribute("Color", state, &this->color));
    }
    if (this->color_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("Alpha", state, &this->color.alpha));
    }
    if (this->cull_chirality_is_set) {
        xml_node_contents.push_back(cull_chirality_to_xml_attribute("Cull", state, &this->cull_chirality));
    }
    if (this->disable_player_cutout_is_set) {
        xml_node_contents.push_back(bool_to_inverted_xml_attribute("CanFade", state, &this->disable_player_cutout));
    }
    if (this->distance_fade_end_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("FadeFar", state, &this->distance_fade_end));
    }
    if (this->distance_fade_start_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("FadeNear", state, &this->distance_fade_start));
    }
    if (this->festival_filter_is_set) {
        xml_node_contents.push_back(festival_filter_to_xml_attribute("Festival", state, &this->festival_filter));
    }
    if (this->guid_is_set) {
        xml_node_contents.push_back(unique_id_to_xml_attribute("GUID", state, &this->guid));
    }
    if (this->is_wall_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("IsWall", state, &this->is_wall));
    }
    if (this->map_display_size_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("MapDisplaySize", state, &this->map_display_size));
    }
    if (this->map_id_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("MapID", state, &this->map_id));
    }
    if (this->map_type_filter_is_set) {
        xml_node_contents.push_back(map_type_filter_to_xml_attribute("MapType", state, &this->map_type_filter));
    }
    if (this->mount_filter_is_set) {
        xml_node_contents.push_back(mount_filter_to_xml_attribute("Mount", state, &this->mount_filter));
    }
    if (this->profession_filter_is_set) {
        xml_node_contents.push_back(profession_filter_to_xml_attribute("Profession", state, &this->profession_filter));
    }
    if (this->render_ingame_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("IngameVisibility", state, &this->render_ingame));
    }
    if (this->render_on_map_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("MapVisibility", state, &this->render_on_map));
    }
    if (this->render_on_minimap_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("MinimapVisibility", state, &this->render_on_minimap));
    }
    if (this->schedule_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("Schedule", state, &this->schedule));
    }
    if (this->schedule_duration_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("ScheduleDuration", state, &this->schedule_duration));
    }
    if (this->specialization_filter_is_set) {
        xml_node_contents.push_back(specialization_filter_to_xml_attribute("Specialization", state, &this->specialization_filter));
    }
    if (this->species_filter_is_set) {
        xml_node_contents.push_back(species_filter_to_xml_attribute("Race", state, &this->species_filter));
    }
    if (this->texture_is_set) {
        xml_node_contents.push_back(image_to_xml_attribute("Texture", state, &this->texture));
    }
    if (this->trail_data_is_set) {
        xml_node_contents.push_back(trail_data_to_xml_attribute("TrailData", state, &this->trail_data, &(this->map_id), &(this->map_id_is_set)));
    }
    if (this->trail_scale_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("TrailScale", state, &this->trail_scale));
    }
    xml_node_contents.push_back("/>");
    return xml_node_contents;
}

waypoint::Trail Trail::as_protobuf(ProtoWriterState* state) const {
    waypoint::Trail proto_trail;
    if (this->achievement_bitmask_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_achievement_bit(val); };
        int_to_proto(this->achievement_bitmask, state, setter);
    }
    if (this->achievement_id_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_achievement_id(val); };
        int_to_proto(this->achievement_id, state, setter);
    }
    if (this->animation_speed_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_animation_speed(val); };
        float_to_proto(this->animation_speed, state, setter);
    }
    if (this->category_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_category(val); };
        do_nothing(this->category, state, setter);
    }
    if (this->color_is_set) {
        std::function<void(waypoint::RGBAColor*)> setter = [&proto_trail](waypoint::RGBAColor* val) { proto_trail.set_allocated_rgba_color(val); };
        color_to_proto(this->color, state, setter);
    }
    if (this->cull_chirality_is_set) {
        std::function<void(waypoint::CullChirality)> setter = [&proto_trail](waypoint::CullChirality val) { proto_trail.set_cull_chirality(val); };
        cull_chirality_to_proto(this->cull_chirality, state, setter);
    }
    if (this->disable_player_cutout_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_disable_player_cutout(val); };
        bool_to_proto(this->disable_player_cutout, state, setter);
    }
    if (this->distance_fade_end_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_distance_fade_end(val); };
        float_to_proto(this->distance_fade_end, state, setter);
    }
    if (this->distance_fade_start_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_distance_fade_start(val); };
        float_to_proto(this->distance_fade_start, state, setter);
    }
    if (this->festival_filter_is_set) {
        std::function<void(waypoint::FestivalFilter*)> setter = [&proto_trail](waypoint::FestivalFilter* val) { proto_trail.set_allocated_festival_filter(val); };
        festival_filter_to_proto(this->festival_filter, state, setter);
    }
    if (this->guid_is_set) {
        std::function<void(std::string)> setter = [&proto_trail](std::string val) { proto_trail.set_guid(val); };
        unique_id_to_proto(this->guid, state, setter);
    }
    if (this->is_wall_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_is_wall(val); };
        bool_to_proto(this->is_wall, state, setter);
    }
    if (this->map_display_size_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_map_display_size(val); };
        int_to_proto(this->map_display_size, state, setter);
    }
    if (this->map_id_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_map_id(val); };
        int_to_proto(this->map_id, state, setter);
    }
    if (this->map_type_filter_is_set) {
        std::function<void(waypoint::MapTypeFilter*)> setter = [&proto_trail](waypoint::MapTypeFilter* val) { proto_trail.set_allocated_map_type_filter(val); };
        map_type_filter_to_proto(this->map_type_filter, state, setter);
    }
    if (this->mount_filter_is_set) {
        std::function<void(waypoint::MountFilter*)> setter = [&proto_trail](waypoint::MountFilter* val) { proto_trail.set_allocated_mount_filter(val); };
        mount_filter_to_proto(this->mount_filter, state, setter);
    }
    if (this->profession_filter_is_set) {
        std::function<void(waypoint::ProfessionFilter*)> setter = [&proto_trail](waypoint::ProfessionFilter* val) { proto_trail.set_allocated_profession_filter(val); };
        profession_filter_to_proto(this->profession_filter, state, setter);
    }
    if (this->render_ingame_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_tentative__render_ingame(val); };
        bool_to_proto(this->render_ingame, state, setter);
    }
    if (this->render_on_map_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_tentative__render_on_map(val); };
        bool_to_proto(this->render_on_map, state, setter);
    }
    if (this->render_on_minimap_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_tentative__render_on_minimap(val); };
        bool_to_proto(this->render_on_minimap, state, setter);
    }
    if (this->schedule_is_set) {
        std::function<void(std::string)> setter = [&proto_trail](std::string val) { proto_trail.set_bhdraft__schedule(val); };
        string_to_proto(this->schedule, state, setter);
    }
    if (this->schedule_duration_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_bhdraft__schedule_duration(val); };
        float_to_proto(this->schedule_duration, state, setter);
    }
    if (this->specialization_filter_is_set) {
        std::function<void(waypoint::SpecializationFilter*)> setter = [&proto_trail](waypoint::SpecializationFilter* val) { proto_trail.set_allocated_specialization_filter(val); };
        specialization_filter_to_proto(this->specialization_filter, state, setter);
    }
    if (this->species_filter_is_set) {
        std::function<void(waypoint::SpeciesFilter*)> setter = [&proto_trail](waypoint::SpeciesFilter* val) { proto_trail.set_allocated_species_filter(val); };
        species_filter_to_proto(this->species_filter, state, setter);
    }
    if (this->texture_is_set) {
        std::function<void(unsigned int)> setter = [&proto_trail](unsigned int val) { proto_trail.set_texture_id(val); };
        image_to_proto(this->texture, state, setter);
    }
    if (this->trail_data_is_set) {
        std::function<void(waypoint::TrailData*)> setter = [&proto_trail](waypoint::TrailData* val) { proto_trail.set_allocated_trail_data(val); };
        trail_data_to_proto(this->trail_data, state, setter);
    }
    if (this->trail_scale_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_scale(val); };
        float_to_proto(this->trail_scale, state, setter);
    }
    return proto_trail;
}

void Trail::parse_protobuf(waypoint::Trail proto_trail, ProtoReaderState* state) {
    if (proto_trail.achievement_bit() != 0) {
        proto_to_int(proto_trail.achievement_bit(), state, &(this->achievement_bitmask), &(this->achievement_bitmask_is_set));
    }
    if (proto_trail.achievement_id() != 0) {
        proto_to_int(proto_trail.achievement_id(), state, &(this->achievement_id), &(this->achievement_id_is_set));
    }
    if (proto_trail.animation_speed() != 0) {
        proto_to_float(proto_trail.animation_speed(), state, &(this->animation_speed), &(this->animation_speed_is_set));
    }
    if (proto_trail.category() != 0) {
        do_nothing(proto_trail.category(), state, &(this->category), &(this->category_is_set));
    }
    if (proto_trail.has_rgba_color()) {
        proto_to_color(proto_trail.rgba_color(), state, &(this->color), &(this->color_is_set));
    }
    if (proto_trail.cull_chirality() != 0) {
        proto_to_cull_chirality(proto_trail.cull_chirality(), state, &(this->cull_chirality), &(this->cull_chirality_is_set));
    }
    if (proto_trail.disable_player_cutout() != 0) {
        proto_to_bool(proto_trail.disable_player_cutout(), state, &(this->disable_player_cutout), &(this->disable_player_cutout_is_set));
    }
    if (proto_trail.distance_fade_end() != 0) {
        proto_to_float(proto_trail.distance_fade_end(), state, &(this->distance_fade_end), &(this->distance_fade_end_is_set));
    }
    if (proto_trail.distance_fade_start() != 0) {
        proto_to_float(proto_trail.distance_fade_start(), state, &(this->distance_fade_start), &(this->distance_fade_start_is_set));
    }
    if (proto_trail.has_festival_filter()) {
        proto_to_festival_filter(proto_trail.festival_filter(), state, &(this->festival_filter), &(this->festival_filter_is_set));
    }
    if (proto_trail.guid() != "") {
        proto_to_unique_id(proto_trail.guid(), state, &(this->guid), &(this->guid_is_set));
    }
    if (proto_trail.is_wall() != 0) {
        proto_to_bool(proto_trail.is_wall(), state, &(this->is_wall), &(this->is_wall_is_set));
    }
    if (proto_trail.map_display_size() != 0) {
        proto_to_int(proto_trail.map_display_size(), state, &(this->map_display_size), &(this->map_display_size_is_set));
    }
    if (proto_trail.map_id() != 0) {
        proto_to_int(proto_trail.map_id(), state, &(this->map_id), &(this->map_id_is_set));
    }
    if (proto_trail.has_map_type_filter()) {
        proto_to_map_type_filter(proto_trail.map_type_filter(), state, &(this->map_type_filter), &(this->map_type_filter_is_set));
    }
    if (proto_trail.has_mount_filter()) {
        proto_to_mount_filter(proto_trail.mount_filter(), state, &(this->mount_filter), &(this->mount_filter_is_set));
    }
    if (proto_trail.has_profession_filter()) {
        proto_to_profession_filter(proto_trail.profession_filter(), state, &(this->profession_filter), &(this->profession_filter_is_set));
    }
    if (proto_trail.tentative__render_ingame() != 0) {
        proto_to_bool(proto_trail.tentative__render_ingame(), state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    if (proto_trail.tentative__render_on_map() != 0) {
        proto_to_bool(proto_trail.tentative__render_on_map(), state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    if (proto_trail.tentative__render_on_minimap() != 0) {
        proto_to_bool(proto_trail.tentative__render_on_minimap(), state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    if (proto_trail.bhdraft__schedule() != "") {
        proto_to_string(proto_trail.bhdraft__schedule(), state, &(this->schedule), &(this->schedule_is_set));
    }
    if (proto_trail.bhdraft__schedule_duration() != 0) {
        proto_to_float(proto_trail.bhdraft__schedule_duration(), state, &(this->schedule_duration), &(this->schedule_duration_is_set));
    }
    if (proto_trail.has_specialization_filter()) {
        proto_to_specialization_filter(proto_trail.specialization_filter(), state, &(this->specialization_filter), &(this->specialization_filter_is_set));
    }
    if (proto_trail.has_species_filter()) {
        proto_to_species_filter(proto_trail.species_filter(), state, &(this->species_filter), &(this->species_filter_is_set));
    }
    if (proto_trail.texture_id() != 0) {
        proto_to_image(proto_trail.texture_id(), state, &(this->texture), &(this->texture_is_set));
    }
    if (proto_trail.has_trail_data()) {
        proto_to_trail_data(proto_trail.trail_data(), state, &(this->trail_data), &(this->trail_data_is_set));
    }
    if (proto_trail.scale() != 0) {
        proto_to_float(proto_trail.scale(), state, &(this->trail_scale), &(this->trail_scale_is_set));
    }
}

////////////////////////////////////////////////////////////////////////////////
// apply_underlay
//
// Transforms this Trail as if this class was overlayed on top of the
// underlay argument.
////////////////////////////////////////////////////////////////////////////////
void Trail::apply_underlay(const Trail& underlay) {
    if (!this->achievement_bitmask_is_set && underlay.achievement_bitmask_is_set) {
        this->achievement_bitmask = underlay.achievement_bitmask;
        this->achievement_bitmask_is_set = true;
    }
    if (!this->achievement_id_is_set && underlay.achievement_id_is_set) {
        this->achievement_id = underlay.achievement_id;
        this->achievement_id_is_set = true;
    }
    if (!this->animation_speed_is_set && underlay.animation_speed_is_set) {
        this->animation_speed = underlay.animation_speed;
        this->animation_speed_is_set = true;
    }
    if (!this->category_is_set && underlay.category_is_set) {
        this->category = underlay.category;
        this->category_is_set = true;
    }
    if (!this->color_is_set && underlay.color_is_set) {
        this->color = underlay.color;
        this->color_is_set = true;
    }
    if (!this->cull_chirality_is_set && underlay.cull_chirality_is_set) {
        this->cull_chirality = underlay.cull_chirality;
        this->cull_chirality_is_set = true;
    }
    if (!this->disable_player_cutout_is_set && underlay.disable_player_cutout_is_set) {
        this->disable_player_cutout = underlay.disable_player_cutout;
        this->disable_player_cutout_is_set = true;
    }
    if (!this->distance_fade_end_is_set && underlay.distance_fade_end_is_set) {
        this->distance_fade_end = underlay.distance_fade_end;
        this->distance_fade_end_is_set = true;
    }
    if (!this->distance_fade_start_is_set && underlay.distance_fade_start_is_set) {
        this->distance_fade_start = underlay.distance_fade_start;
        this->distance_fade_start_is_set = true;
    }
    if (!this->festival_filter_is_set && underlay.festival_filter_is_set) {
        this->festival_filter = underlay.festival_filter;
        this->festival_filter_is_set = true;
    }
    if (!this->guid_is_set && underlay.guid_is_set) {
        this->guid = underlay.guid;
        this->guid_is_set = true;
    }
    if (!this->is_wall_is_set && underlay.is_wall_is_set) {
        this->is_wall = underlay.is_wall;
        this->is_wall_is_set = true;
    }
    if (!this->map_display_size_is_set && underlay.map_display_size_is_set) {
        this->map_display_size = underlay.map_display_size;
        this->map_display_size_is_set = true;
    }
    if (!this->map_id_is_set && underlay.map_id_is_set) {
        this->map_id = underlay.map_id;
        this->map_id_is_set = true;
    }
    if (!this->map_type_filter_is_set && underlay.map_type_filter_is_set) {
        this->map_type_filter = underlay.map_type_filter;
        this->map_type_filter_is_set = true;
    }
    if (!this->mount_filter_is_set && underlay.mount_filter_is_set) {
        this->mount_filter = underlay.mount_filter;
        this->mount_filter_is_set = true;
    }
    if (!this->profession_filter_is_set && underlay.profession_filter_is_set) {
        this->profession_filter = underlay.profession_filter;
        this->profession_filter_is_set = true;
    }
    if (!this->render_ingame_is_set && underlay.render_ingame_is_set) {
        this->render_ingame = underlay.render_ingame;
        this->render_ingame_is_set = true;
    }
    if (!this->render_on_map_is_set && underlay.render_on_map_is_set) {
        this->render_on_map = underlay.render_on_map;
        this->render_on_map_is_set = true;
    }
    if (!this->render_on_minimap_is_set && underlay.render_on_minimap_is_set) {
        this->render_on_minimap = underlay.render_on_minimap;
        this->render_on_minimap_is_set = true;
    }
    if (!this->schedule_is_set && underlay.schedule_is_set) {
        this->schedule = underlay.schedule;
        this->schedule_is_set = true;
    }
    if (!this->schedule_duration_is_set && underlay.schedule_duration_is_set) {
        this->schedule_duration = underlay.schedule_duration;
        this->schedule_duration_is_set = true;
    }
    if (!this->specialization_filter_is_set && underlay.specialization_filter_is_set) {
        this->specialization_filter = underlay.specialization_filter;
        this->specialization_filter_is_set = true;
    }
    if (!this->species_filter_is_set && underlay.species_filter_is_set) {
        this->species_filter = underlay.species_filter;
        this->species_filter_is_set = true;
    }
    if (!this->texture_is_set && underlay.texture_is_set) {
        this->texture = underlay.texture;
        this->texture_is_set = true;
    }
    if (!this->trail_data_is_set && underlay.trail_data_is_set) {
        this->trail_data = underlay.trail_data;
        this->trail_data_is_set = true;
    }
    if (!this->trail_scale_is_set && underlay.trail_scale_is_set) {
        this->trail_scale = underlay.trail_scale;
        this->trail_scale_is_set = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
// apply_overlay
//
// Transforms this Trail as if the overlay argument were overlayed on
// top of this class.
////////////////////////////////////////////////////////////////////////////////
void Trail::apply_overlay(const Trail& overlay) {
    if (overlay.achievement_bitmask_is_set) {
        this->achievement_bitmask = overlay.achievement_bitmask;
        this->achievement_bitmask_is_set = true;
    }
    if (overlay.achievement_id_is_set) {
        this->achievement_id = overlay.achievement_id;
        this->achievement_id_is_set = true;
    }
    if (overlay.animation_speed_is_set) {
        this->animation_speed = overlay.animation_speed;
        this->animation_speed_is_set = true;
    }
    if (overlay.category_is_set) {
        this->category = overlay.category;
        this->category_is_set = true;
    }
    if (overlay.color_is_set) {
        this->color = overlay.color;
        this->color_is_set = true;
    }
    if (overlay.cull_chirality_is_set) {
        this->cull_chirality = overlay.cull_chirality;
        this->cull_chirality_is_set = true;
    }
    if (overlay.disable_player_cutout_is_set) {
        this->disable_player_cutout = overlay.disable_player_cutout;
        this->disable_player_cutout_is_set = true;
    }
    if (overlay.distance_fade_end_is_set) {
        this->distance_fade_end = overlay.distance_fade_end;
        this->distance_fade_end_is_set = true;
    }
    if (overlay.distance_fade_start_is_set) {
        this->distance_fade_start = overlay.distance_fade_start;
        this->distance_fade_start_is_set = true;
    }
    if (overlay.festival_filter_is_set) {
        this->festival_filter = overlay.festival_filter;
        this->festival_filter_is_set = true;
    }
    if (overlay.guid_is_set) {
        this->guid = overlay.guid;
        this->guid_is_set = true;
    }
    if (overlay.is_wall_is_set) {
        this->is_wall = overlay.is_wall;
        this->is_wall_is_set = true;
    }
    if (overlay.map_display_size_is_set) {
        this->map_display_size = overlay.map_display_size;
        this->map_display_size_is_set = true;
    }
    if (overlay.map_id_is_set) {
        this->map_id = overlay.map_id;
        this->map_id_is_set = true;
    }
    if (overlay.map_type_filter_is_set) {
        this->map_type_filter = overlay.map_type_filter;
        this->map_type_filter_is_set = true;
    }
    if (overlay.mount_filter_is_set) {
        this->mount_filter = overlay.mount_filter;
        this->mount_filter_is_set = true;
    }
    if (overlay.profession_filter_is_set) {
        this->profession_filter = overlay.profession_filter;
        this->profession_filter_is_set = true;
    }
    if (overlay.render_ingame_is_set) {
        this->render_ingame = overlay.render_ingame;
        this->render_ingame_is_set = true;
    }
    if (overlay.render_on_map_is_set) {
        this->render_on_map = overlay.render_on_map;
        this->render_on_map_is_set = true;
    }
    if (overlay.render_on_minimap_is_set) {
        this->render_on_minimap = overlay.render_on_minimap;
        this->render_on_minimap_is_set = true;
    }
    if (overlay.schedule_is_set) {
        this->schedule = overlay.schedule;
        this->schedule_is_set = true;
    }
    if (overlay.schedule_duration_is_set) {
        this->schedule_duration = overlay.schedule_duration;
        this->schedule_duration_is_set = true;
    }
    if (overlay.specialization_filter_is_set) {
        this->specialization_filter = overlay.specialization_filter;
        this->specialization_filter_is_set = true;
    }
    if (overlay.species_filter_is_set) {
        this->species_filter = overlay.species_filter;
        this->species_filter_is_set = true;
    }
    if (overlay.texture_is_set) {
        this->texture = overlay.texture;
        this->texture_is_set = true;
    }
    if (overlay.trail_data_is_set) {
        this->trail_data = overlay.trail_data;
        this->trail_data_is_set = true;
    }
    if (overlay.trail_scale_is_set) {
        this->trail_scale = overlay.trail_scale;
        this->trail_scale_is_set = true;
    }
}
