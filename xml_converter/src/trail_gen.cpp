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

vector<string> Trail::as_xml(XMLWriterState* state) const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<Trail ");
    if (this->achievement_bit_index_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("AchievementBit", state, &this->achievement_bit_index));
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
        xml_node_contents.push_back(bool_to_inverted_xml_attribute("IngameVisibility", state, &this->render_ingame));
    }
    if (this->render_on_map_is_set) {
        xml_node_contents.push_back(bool_to_inverted_xml_attribute("MapVisibility", state, &this->render_on_map));
    }
    if (this->render_on_minimap_is_set) {
        xml_node_contents.push_back(bool_to_inverted_xml_attribute("MinimapVisibility", state, &this->render_on_minimap));
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

guildpoint::Trail Trail::as_protobuf(ProtoWriterState* state) const {
    guildpoint::Trail proto_trail;
    if (this->achievement_bit_index_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_achievement_bit_index(val); };
        int_to_proto(this->achievement_bit_index, state, setter);
    }
    if (this->achievement_id_is_set) {
        std::function<void(int)> setter = [&proto_trail](int val) { proto_trail.set_achievement_id(val); };
        int_to_proto(this->achievement_id, state, setter);
    }
    if (this->color_is_set) {
        std::function<void(uint32_t)> setter = [&proto_trail](uint32_t val) { proto_trail.set_rgba_color(val); };
        color_to_proto(this->color, state, setter);
    }
    if (this->cull_chirality_is_set) {
        std::function<void(guildpoint::CullChirality)> setter = [&proto_trail](guildpoint::CullChirality val) { proto_trail.set_cull_chirality(val); };
        cull_chirality_to_proto(this->cull_chirality, state, setter);
    }
    if (this->disable_player_cutout_is_set) {
        std::function<void(bool)> setter = [&proto_trail](bool val) { proto_trail.set_disable_player_cutout(val); };
        bool_to_proto(this->disable_player_cutout, state, setter);
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
    if (this->schedule_is_set) {
        std::function<void(std::string)> setter = [&proto_trail](std::string val) { proto_trail.set_bhdraft__schedule(val); };
        string_to_proto(this->schedule, state, setter);
    }
    if (this->schedule_duration_is_set) {
        std::function<void(float)> setter = [&proto_trail](float val) { proto_trail.set_bhdraft__schedule_duration(val); };
        float_to_proto(this->schedule_duration, state, setter);
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
    if (proto_trail.achievement_bit_index() != 0) {
        proto_to_int(proto_trail.achievement_bit_index(), state, &(this->achievement_bit_index), &(this->achievement_bit_index_is_set));
    }
    if (proto_trail.achievement_id() != 0) {
        proto_to_int(proto_trail.achievement_id(), state, &(this->achievement_id), &(this->achievement_id_is_set));
    }
    if (proto_trail.rgba_color() != 0) {
        proto_to_color(proto_trail.rgba_color(), state, &(this->color), &(this->color_is_set));
    }
    if (proto_trail.cull_chirality() != 0) {
        proto_to_cull_chirality(proto_trail.cull_chirality(), state, &(this->cull_chirality), &(this->cull_chirality_is_set));
    }
    if (proto_trail.disable_player_cutout() != 0) {
        proto_to_bool(proto_trail.disable_player_cutout(), state, &(this->disable_player_cutout), &(this->disable_player_cutout_is_set));
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
}
