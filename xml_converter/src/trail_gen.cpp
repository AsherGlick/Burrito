#include "trail_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>

#include "attribute/bool.hpp"
#include "attribute/float.hpp"
#include "attribute/int.hpp"
#include "attribute/string.hpp"
#include "guildpoint.pb.h"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "string_helper.hpp"

using namespace std;

string Trail::classname() {
    return "Trail";
}

bool Trail::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, XMLReaderState* state) {
    string attributename;
    attributename = normalize(get_attribute_name(attribute));
    if (attributename == "achievementbit") {
        xml_attribute_to_int(attribute, errors, state, &(this->achievement_bit_index), &(this->achievement_bit_index_is_set));
    }
    else if (attributename == "achievementbitindex") {
        xml_attribute_to_int(attribute, errors, state, &(this->achievement_bit_index), &(this->achievement_bit_index_is_set));
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
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    else if (attributename == "bhingamevisibility") {
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    else if (attributename == "mapvisibility") {
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    else if (attributename == "bhmapvisibility") {
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    else if (attributename == "minimapvisibility") {
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    else if (attributename == "bhminimapvisibility") {
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
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

rapidxml::xml_node<char>* Trail::as_xml(XMLWriterState* state) const {
    rapidxml::xml_node<char>* xml_node = state->doc->allocate_node(rapidxml::node_element, "Trail");

    if (this->achievement_bit_index_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("AchievementBit", state->doc->allocate_string(val.c_str()))); };
        int_to_xml_attribute(state, &this->achievement_bit_index, setter);
    }
    if (this->achievement_id_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("AchievementId", state->doc->allocate_string(val.c_str()))); };
        int_to_xml_attribute(state, &this->achievement_id, setter);
    }
    if (this->animation_speed_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("AnimSpeed", state->doc->allocate_string(val.c_str()))); };
        float_to_xml_attribute(state, &this->animation_speed, setter);
    }
    if (this->category_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Type", state->doc->allocate_string(val.c_str()))); };
        marker_category_to_xml_attribute(state, &this->category, setter);
    }
    if (this->color_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Color", state->doc->allocate_string(val.c_str()))); };
        color_to_xml_attribute(state, &this->color, setter);
    }
    if (this->color_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Alpha", state->doc->allocate_string(val.c_str()))); };
        float_to_xml_attribute(state, &this->color.alpha, setter);
    }
    if (this->cull_chirality_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Cull", state->doc->allocate_string(val.c_str()))); };
        cull_chirality_to_xml_attribute(state, &this->cull_chirality, setter);
    }
    if (this->disable_player_cutout_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("CanFade", state->doc->allocate_string(val.c_str()))); };
        bool_to_inverted_xml_attribute(state, &this->disable_player_cutout, setter);
    }
    if (this->distance_fade_end_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("FadeFar", state->doc->allocate_string(val.c_str()))); };
        float_to_xml_attribute(state, &this->distance_fade_end, setter);
    }
    if (this->distance_fade_start_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("FadeNear", state->doc->allocate_string(val.c_str()))); };
        float_to_xml_attribute(state, &this->distance_fade_start, setter);
    }
    if (this->festival_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Festival", state->doc->allocate_string(val.c_str()))); };
        festival_filter_to_xml_attribute(state, &this->festival_filter, setter);
    }
    if (this->guid_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("GUID", state->doc->allocate_string(val.c_str()))); };
        unique_id_to_xml_attribute(state, &this->guid, setter);
    }
    if (this->is_wall_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("IsWall", state->doc->allocate_string(val.c_str()))); };
        bool_to_xml_attribute(state, &this->is_wall, setter);
    }
    if (this->map_display_size_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MapDisplaySize", state->doc->allocate_string(val.c_str()))); };
        int_to_xml_attribute(state, &this->map_display_size, setter);
    }
    if (this->map_id_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MapID", state->doc->allocate_string(val.c_str()))); };
        int_to_xml_attribute(state, &this->map_id, setter);
    }
    if (this->map_type_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MapType", state->doc->allocate_string(val.c_str()))); };
        map_type_filter_to_xml_attribute(state, &this->map_type_filter, setter);
    }
    if (this->mount_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Mount", state->doc->allocate_string(val.c_str()))); };
        mount_filter_to_xml_attribute(state, &this->mount_filter, setter);
    }
    if (this->profession_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Profession", state->doc->allocate_string(val.c_str()))); };
        profession_filter_to_xml_attribute(state, &this->profession_filter, setter);
    }
    if (this->render_ingame_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("IngameVisibility", state->doc->allocate_string(val.c_str()))); };
        bool_to_inverted_xml_attribute(state, &this->render_ingame, setter);
    }
    if (this->render_on_map_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MapVisibility", state->doc->allocate_string(val.c_str()))); };
        bool_to_inverted_xml_attribute(state, &this->render_on_map, setter);
    }
    if (this->render_on_minimap_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MinimapVisibility", state->doc->allocate_string(val.c_str()))); };
        bool_to_inverted_xml_attribute(state, &this->render_on_minimap, setter);
    }
    if (this->schedule_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Schedule", state->doc->allocate_string(val.c_str()))); };
        string_to_xml_attribute(state, &this->schedule, setter);
    }
    if (this->schedule_duration_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("ScheduleDuration", state->doc->allocate_string(val.c_str()))); };
        float_to_xml_attribute(state, &this->schedule_duration, setter);
    }
    if (this->specialization_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Specialization", state->doc->allocate_string(val.c_str()))); };
        specialization_filter_to_xml_attribute(state, &this->specialization_filter, setter);
    }
    if (this->species_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Race", state->doc->allocate_string(val.c_str()))); };
        species_filter_to_xml_attribute(state, &this->species_filter, setter);
    }
    if (this->texture_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Texture", state->doc->allocate_string(val.c_str()))); };
        image_to_xml_attribute(state, &this->texture, setter);
    }
    if (this->trail_data_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("TrailData", state->doc->allocate_string(val.c_str()))); };
        trail_data_to_xml_attribute(state, &this->trail_data, &(this->map_id), &(this->map_id_is_set), setter);
    }
    if (this->trail_scale_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("TrailScale", state->doc->allocate_string(val.c_str()))); };
        float_to_xml_attribute(state, &this->trail_scale, setter);
    }
    return xml_node;
}

// The following attributes are not yet supported in Burrito
// and are not written to or read from the protobuf file:
//  achievement_bit_index
//  achievement_id
//  animation_speed
//  disable_player_cutout
//  distance_fade_end
//  distance_fade_start
//  map_display_size
//  schedule
//  schedule_duration
//  trail_scale

guildpoint::Trail Trail::as_protobuf(ProtoWriterState* state) const {
    guildpoint::Trail proto_trail;
    if (this->color_is_set) {
        std::function<void(uint32_t)> setter = [&proto_trail](uint32_t val) { proto_trail.set_rgba_color(val); };
        color_to_proto(this->color, state, setter);
    }
    if (this->cull_chirality_is_set) {
        std::function<void(guildpoint::CullChirality)> setter = [&proto_trail](guildpoint::CullChirality val) { proto_trail.set_cull_chirality(val); };
        cull_chirality_to_proto(this->cull_chirality, state, setter);
    }
    if (this->festival_filter_is_set) {
        std::function<void(guildpoint::FestivalFilter*)> setter = [&proto_trail](guildpoint::FestivalFilter* val) { proto_trail.set_allocated_festival_filter(val); };
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
    if (this->map_id_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_map_id(val); };
        int_to_proto(this->map_id, state, setter);
    }
    if (this->map_type_filter_is_set) {
        std::function<void(guildpoint::MapTypeFilter*)> setter = [&proto_trail](guildpoint::MapTypeFilter* val) { proto_trail.set_allocated_map_type_filter(val); };
        map_type_filter_to_proto(this->map_type_filter, state, setter);
    }
    if (this->mount_filter_is_set) {
        std::function<void(guildpoint::MountFilter*)> setter = [&proto_trail](guildpoint::MountFilter* val) { proto_trail.set_allocated_mount_filter(val); };
        mount_filter_to_proto(this->mount_filter, state, setter);
    }
    if (this->profession_filter_is_set) {
        std::function<void(guildpoint::ProfessionFilter*)> setter = [&proto_trail](guildpoint::ProfessionFilter* val) { proto_trail.set_allocated_profession_filter(val); };
        profession_filter_to_proto(this->profession_filter, state, setter);
    }
    if (this->render_ingame_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_is_hidden_ingame(val); };
        bool_to_proto(this->render_ingame, state, setter);
    }
    if (this->render_on_map_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_is_hidden_on_map(val); };
        bool_to_proto(this->render_on_map, state, setter);
    }
    if (this->render_on_minimap_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_is_hidden_on_minimap(val); };
        bool_to_proto(this->render_on_minimap, state, setter);
    }
    if (this->specialization_filter_is_set) {
        std::function<void(guildpoint::SpecializationFilter*)> setter = [&proto_trail](guildpoint::SpecializationFilter* val) { proto_trail.set_allocated_specialization_filter(val); };
        specialization_filter_to_proto(this->specialization_filter, state, setter);
    }
    if (this->species_filter_is_set) {
        std::function<void(guildpoint::SpeciesFilter*)> setter = [&proto_trail](guildpoint::SpeciesFilter* val) { proto_trail.set_allocated_species_filter(val); };
        species_filter_to_proto(this->species_filter, state, setter);
    }
    if (this->texture_is_set) {
        std::function<void(unsigned int)> setter = [&proto_trail](unsigned int val) { proto_trail.set_texture_id(val); };
        image_to_proto(this->texture, state, setter);
    }
    if (this->trail_data_is_set) {
        std::function<void(guildpoint::TrailData*)> setter = [&proto_trail](guildpoint::TrailData* val) { proto_trail.set_allocated_trail_data(val); };
        trail_data_to_proto(this->trail_data, state, setter);
    }
    return proto_trail;
}

void Trail::parse_protobuf(guildpoint::Trail proto_trail, ProtoReaderState* state) {
    if (proto_trail.rgba_color() != 0) {
        proto_to_color(proto_trail.rgba_color(), state, &(this->color), &(this->color_is_set));
    }
    if (proto_trail.cull_chirality() != 0) {
        proto_to_cull_chirality(proto_trail.cull_chirality(), state, &(this->cull_chirality), &(this->cull_chirality_is_set));
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
    if (proto_trail.is_hidden_ingame() != 0) {
        proto_to_bool(proto_trail.is_hidden_ingame(), state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    if (proto_trail.is_hidden_on_map() != 0) {
        proto_to_bool(proto_trail.is_hidden_on_map(), state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    if (proto_trail.is_hidden_on_minimap() != 0) {
        proto_to_bool(proto_trail.is_hidden_on_minimap(), state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
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
}
