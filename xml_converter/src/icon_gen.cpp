#include "icon_gen.hpp"

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

string Icon::classname() {
    return "POI";
}

bool Icon::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, XMLReaderState* state) {
    string attributename;
    attributename = get_attribute_name(attribute);
    if (attributename == "achievementbit") {
        Attribute::Int::from_xml_attribute(attribute, errors, state, &(this->achievement_bit_index), &(this->achievement_bit_index_is_set));
    }
    else if (attributename == "achievementbitindex") {
        Attribute::Int::from_xml_attribute(attribute, errors, state, &(this->achievement_bit_index), &(this->achievement_bit_index_is_set));
    }
    else if (attributename == "achievementid") {
        Attribute::Int::from_xml_attribute(attribute, errors, state, &(this->achievement_id), &(this->achievement_id_is_set));
    }
    else if (attributename == "autotrigger") {
        Attribute::Bool::from_xml_attribute(attribute, errors, state, &(this->auto_trigger), &(this->auto_trigger_is_set));
    }
    else if (attributename == "bounce") {
        Attribute::Bounce::from_xml_attribute(attribute, errors, state, &(this->bounce), &(this->bounce_is_set));
    }
    else if (attributename == "bouncedelay") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->bounce_delay), &(this->bounce_delay_is_set));
    }
    else if (attributename == "bounceduration") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->bounce_duration), &(this->bounce_duration_is_set));
    }
    else if (attributename == "bounceheight") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->bounce_height), &(this->bounce_height_is_set));
    }
    else if (attributename == "type") {
        Attribute::MarkerCategory::from_xml_attribute(attribute, errors, state, &(this->category), &(this->category_is_set));
    }
    else if (attributename == "category") {
        Attribute::MarkerCategory::from_xml_attribute(attribute, errors, state, &(this->category), &(this->category_is_set));
    }
    else if (attributename == "color") {
        Attribute::Color::from_xml_attribute(attribute, errors, state, &(this->color), &(this->color_is_set));
    }
    else if (attributename == "bhcolor") {
        Attribute::Color::from_xml_attribute(attribute, errors, state, &(this->color), &(this->color_is_set));
    }
    else if (attributename == "alpha") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->color.alpha), &(this->color_is_set));
    }
    else if (attributename == "blue") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->color.blue), &(this->color_is_set));
    }
    else if (attributename == "green") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->color.green), &(this->color_is_set));
    }
    else if (attributename == "red") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->color.red), &(this->color_is_set));
    }
    else if (attributename == "scaleonmapwithzoom") {
        Attribute::InvertBool::from_xml_attribute(attribute, errors, state, &(this->constant_size_on_map), &(this->constant_size_on_map_is_set));
    }
    else if (attributename == "copy") {
        Attribute::String::from_xml_attribute(attribute, errors, state, &(this->copy_clipboard), &(this->copy_clipboard_is_set));
    }
    else if (attributename == "copyclipboard") {
        Attribute::String::from_xml_attribute(attribute, errors, state, &(this->copy_clipboard), &(this->copy_clipboard_is_set));
    }
    else if (attributename == "copymessage") {
        Attribute::String::from_xml_attribute(attribute, errors, state, &(this->copy_message), &(this->copy_message_is_set));
    }
    else if (attributename == "cull") {
        Attribute::CullChirality::from_xml_attribute(attribute, errors, state, &(this->cull_chirality), &(this->cull_chirality_is_set));
    }
    else if (attributename == "canfade") {
        Attribute::InvertBool::from_xml_attribute(attribute, errors, state, &(this->disable_player_cutout), &(this->disable_player_cutout_is_set));
    }
    else if (attributename == "fadefar") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->distance_fade_end), &(this->distance_fade_end_is_set));
    }
    else if (attributename == "distancefadeend") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->distance_fade_end), &(this->distance_fade_end_is_set));
    }
    else if (attributename == "fadenear") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->distance_fade_start), &(this->distance_fade_start_is_set));
    }
    else if (attributename == "distancefadestart") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->distance_fade_start), &(this->distance_fade_start_is_set));
    }
    else if (attributename == "rotate") {
        Attribute::EulerRotation::from_xml_attribute(attribute, errors, state, &(this->euler_rotation), &(this->euler_rotation_is_set));
    }
    else if (attributename == "rotatex") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->euler_rotation.x_rotation), &(this->euler_rotation_is_set));
    }
    else if (attributename == "rotatey") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->euler_rotation.y_rotation), &(this->euler_rotation_is_set));
    }
    else if (attributename == "rotatez") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->euler_rotation.z_rotation), &(this->euler_rotation_is_set));
    }
    else if (attributename == "festival") {
        Attribute::FestivalFilter::from_xml_attribute(attribute, errors, state, &(this->festival_filter), &(this->festival_filter_is_set));
    }
    else if (attributename == "guid") {
        Attribute::UniqueId::from_xml_attribute(attribute, errors, state, &(this->guid), &(this->guid_is_set));
    }
    else if (attributename == "hascountdown") {
        Attribute::Bool::from_xml_attribute(attribute, errors, state, &(this->has_countdown), &(this->has_countdown_is_set));
    }
    else if (attributename == "heightoffset") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->height_offset), &(this->height_offset_is_set));
    }
    else if (attributename == "bhheightoffset") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->height_offset), &(this->height_offset_is_set));
    }
    else if (attributename == "hide") {
        Attribute::MarkerCategory::from_xml_attribute(attribute, errors, state, &(this->hide_category), &(this->hide_category_is_set));
    }
    else if (attributename == "iconfile") {
        Attribute::Image::from_xml_attribute(attribute, errors, state, &(this->icon), &(this->icon_is_set));
    }
    else if (attributename == "iconsize") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->icon_size), &(this->icon_size_is_set));
    }
    else if (attributename == "info") {
        Attribute::String::from_xml_attribute(attribute, errors, state, &(this->info_message), &(this->info_message_is_set));
    }
    else if (attributename == "invertbehavior") {
        Attribute::Bool::from_xml_attribute(attribute, errors, state, &(this->invert_visibility), &(this->invert_visibility_is_set));
    }
    else if (attributename == "mapdisplaysize") {
        Attribute::Int::from_xml_attribute(attribute, errors, state, &(this->map_display_size), &(this->map_display_size_is_set));
    }
    else if (attributename == "mapid") {
        Attribute::Int::from_xml_attribute(attribute, errors, state, &(this->map_id), &(this->map_id_is_set));
    }
    else if (attributename == "maptype") {
        Attribute::MapTypeFilter::from_xml_attribute(attribute, errors, state, &(this->map_type_filter), &(this->map_type_filter_is_set));
    }
    else if (attributename == "maxsize") {
        Attribute::Int::from_xml_attribute(attribute, errors, state, &(this->maximum_size_on_screen), &(this->maximum_size_on_screen_is_set));
    }
    else if (attributename == "minsize") {
        Attribute::Int::from_xml_attribute(attribute, errors, state, &(this->minimum_size_on_screen), &(this->minimum_size_on_screen_is_set));
    }
    else if (attributename == "mount") {
        Attribute::MountFilter::from_xml_attribute(attribute, errors, state, &(this->mount_filter), &(this->mount_filter_is_set));
    }
    else if (attributename == "position") {
        Attribute::Position::from_xml_attribute(attribute, errors, state, &(this->position), &(this->position_is_set));
    }
    else if (attributename == "xpos") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->position.x_position), &(this->position_is_set));
    }
    else if (attributename == "positionx") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->position.x_position), &(this->position_is_set));
    }
    else if (attributename == "ypos") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->position.y_position), &(this->position_is_set));
    }
    else if (attributename == "positiony") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->position.y_position), &(this->position_is_set));
    }
    else if (attributename == "zpos") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->position.z_position), &(this->position_is_set));
    }
    else if (attributename == "positionz") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->position.z_position), &(this->position_is_set));
    }
    else if (attributename == "profession") {
        Attribute::ProfessionFilter::from_xml_attribute(attribute, errors, state, &(this->profession_filter), &(this->profession_filter_is_set));
    }
    else if (attributename == "ingamevisibility") {
        Attribute::InvertBool::from_xml_attribute(attribute, errors, state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    else if (attributename == "bhingamevisibility") {
        Attribute::InvertBool::from_xml_attribute(attribute, errors, state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    else if (attributename == "mapvisibility") {
        Attribute::InvertBool::from_xml_attribute(attribute, errors, state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    else if (attributename == "bhmapvisibility") {
        Attribute::InvertBool::from_xml_attribute(attribute, errors, state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    else if (attributename == "minimapvisibility") {
        Attribute::InvertBool::from_xml_attribute(attribute, errors, state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    else if (attributename == "bhminimapvisibility") {
        Attribute::InvertBool::from_xml_attribute(attribute, errors, state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    else if (attributename == "behavior") {
        Attribute::ResetBehavior::from_xml_attribute(attribute, errors, state, &(this->reset_behavior), &(this->reset_behavior_is_set));
    }
    else if (attributename == "resetlength") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->reset_length), &(this->reset_length_is_set));
    }
    else if (attributename == "schedule") {
        Attribute::String::from_xml_attribute(attribute, errors, state, &(this->schedule), &(this->schedule_is_set));
    }
    else if (attributename == "scheduleduration") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->schedule_duration), &(this->schedule_duration_is_set));
    }
    else if (attributename == "show") {
        Attribute::MarkerCategory::from_xml_attribute(attribute, errors, state, &(this->show_category), &(this->show_category_is_set));
    }
    else if (attributename == "specialization") {
        Attribute::SpecializationFilter::from_xml_attribute(attribute, errors, state, &(this->specialization_filter), &(this->specialization_filter_is_set));
    }
    else if (attributename == "race") {
        Attribute::SpeciesFilter::from_xml_attribute(attribute, errors, state, &(this->species_filter), &(this->species_filter_is_set));
    }
    else if (attributename == "species") {
        Attribute::SpeciesFilter::from_xml_attribute(attribute, errors, state, &(this->species_filter), &(this->species_filter_is_set));
    }
    else if (attributename == "toggle") {
        Attribute::MarkerCategory::from_xml_attribute(attribute, errors, state, &(this->toggle_category), &(this->toggle_category_is_set));
    }
    else if (attributename == "togglecategory") {
        Attribute::MarkerCategory::from_xml_attribute(attribute, errors, state, &(this->toggle_category), &(this->toggle_category_is_set));
    }
    else if (attributename == "tipdescription") {
        Attribute::String::from_xml_attribute(attribute, errors, state, &(this->tooltip_description), &(this->tooltip_description_is_set));
    }
    else if (attributename == "tipname") {
        Attribute::String::from_xml_attribute(attribute, errors, state, &(this->tooltip_name), &(this->tooltip_name_is_set));
    }
    else if (attributename == "triggerrange") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->trigger_range), &(this->trigger_range_is_set));
    }
    else if (attributename == "inforange") {
        Attribute::Float::from_xml_attribute(attribute, errors, state, &(this->trigger_range), &(this->trigger_range_is_set));
    }
    else {
        return false;
    }
    return true;
}

bool Icon::validate_attributes_of_type_marker_category() {
    // TODO: validate "category"
    // TODO: validate "hide_category"
    // TODO: validate "show_category"
    // TODO: validate "toggle_category"
    return true;
}

rapidxml::xml_node<char>* Icon::as_xml(XMLWriterState* state) const {
    rapidxml::xml_node<char>* xml_node = state->doc->allocate_node(rapidxml::node_element, "POI");

    if (this->achievement_bit_index_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("AchievementBit", state->doc->allocate_string(val.c_str()))); };
        Attribute::Int::to_xml_attribute(state, &this->achievement_bit_index, setter);
    }
    if (this->achievement_id_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("AchievementId", state->doc->allocate_string(val.c_str()))); };
        Attribute::Int::to_xml_attribute(state, &this->achievement_id, setter);
    }
    if (this->auto_trigger_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("AutoTrigger", state->doc->allocate_string(val.c_str()))); };
        Attribute::Bool::to_xml_attribute(state, &this->auto_trigger, setter);
    }
    if (this->bounce_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Bounce", state->doc->allocate_string(val.c_str()))); };
        Attribute::Bounce::to_xml_attribute(state, &this->bounce, setter);
    }
    if (this->bounce_delay_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("BounceDelay", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->bounce_delay, setter);
    }
    if (this->bounce_duration_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("BounceDuration", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->bounce_duration, setter);
    }
    if (this->bounce_height_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("BounceHeight", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->bounce_height, setter);
    }
    if (this->category_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Type", state->doc->allocate_string(val.c_str()))); };
        Attribute::MarkerCategory::to_xml_attribute(state, &this->category, setter);
    }
    if (this->color_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Color", state->doc->allocate_string(val.c_str()))); };
        Attribute::Color::to_xml_attribute(state, &this->color, setter);
    }
    if (this->color_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Alpha", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->color.alpha, setter);
    }
    if (this->constant_size_on_map_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("ScaleOnMapWithZoom", state->doc->allocate_string(val.c_str()))); };
        Attribute::InvertBool::to_xml_attribute(state, &this->constant_size_on_map, setter);
    }
    if (this->copy_clipboard_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Copy", state->doc->allocate_string(val.c_str()))); };
        Attribute::String::to_xml_attribute(state, &this->copy_clipboard, setter);
    }
    if (this->copy_message_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("CopyMessage", state->doc->allocate_string(val.c_str()))); };
        Attribute::String::to_xml_attribute(state, &this->copy_message, setter);
    }
    if (this->cull_chirality_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Cull", state->doc->allocate_string(val.c_str()))); };
        Attribute::CullChirality::to_xml_attribute(state, &this->cull_chirality, setter);
    }
    if (this->disable_player_cutout_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("CanFade", state->doc->allocate_string(val.c_str()))); };
        Attribute::InvertBool::to_xml_attribute(state, &this->disable_player_cutout, setter);
    }
    if (this->distance_fade_end_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("FadeFar", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->distance_fade_end, setter);
    }
    if (this->distance_fade_start_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("FadeNear", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->distance_fade_start, setter);
    }
    if (this->festival_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Festival", state->doc->allocate_string(val.c_str()))); };
        Attribute::FestivalFilter::to_xml_attribute(state, &this->festival_filter, setter);
    }
    if (this->guid_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("GUID", state->doc->allocate_string(val.c_str()))); };
        Attribute::UniqueId::to_xml_attribute(state, &this->guid, setter);
    }
    if (this->has_countdown_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("HasCountdown", state->doc->allocate_string(val.c_str()))); };
        Attribute::Bool::to_xml_attribute(state, &this->has_countdown, setter);
    }
    if (this->height_offset_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("HeightOffset", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->height_offset, setter);
    }
    if (this->hide_category_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Hide", state->doc->allocate_string(val.c_str()))); };
        Attribute::MarkerCategory::to_xml_attribute(state, &this->hide_category, setter);
    }
    if (this->icon_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("IconFile", state->doc->allocate_string(val.c_str()))); };
        Attribute::Image::to_xml_attribute(state, &this->icon, setter);
    }
    if (this->icon_size_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("IconSize", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->icon_size, setter);
    }
    if (this->info_message_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Info", state->doc->allocate_string(val.c_str()))); };
        Attribute::String::to_xml_attribute(state, &this->info_message, setter);
    }
    if (this->invert_visibility_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("InvertBehavior", state->doc->allocate_string(val.c_str()))); };
        Attribute::Bool::to_xml_attribute(state, &this->invert_visibility, setter);
    }
    if (this->map_display_size_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MapDisplaySize", state->doc->allocate_string(val.c_str()))); };
        Attribute::Int::to_xml_attribute(state, &this->map_display_size, setter);
    }
    if (this->map_id_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MapID", state->doc->allocate_string(val.c_str()))); };
        Attribute::Int::to_xml_attribute(state, &this->map_id, setter);
    }
    if (this->map_type_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MapType", state->doc->allocate_string(val.c_str()))); };
        Attribute::MapTypeFilter::to_xml_attribute(state, &this->map_type_filter, setter);
    }
    if (this->maximum_size_on_screen_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MaxSize", state->doc->allocate_string(val.c_str()))); };
        Attribute::Int::to_xml_attribute(state, &this->maximum_size_on_screen, setter);
    }
    if (this->minimum_size_on_screen_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MinSize", state->doc->allocate_string(val.c_str()))); };
        Attribute::Int::to_xml_attribute(state, &this->minimum_size_on_screen, setter);
    }
    if (this->mount_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Mount", state->doc->allocate_string(val.c_str()))); };
        Attribute::MountFilter::to_xml_attribute(state, &this->mount_filter, setter);
    }
    if (this->position_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("XPos", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->position.x_position, setter);
    }
    if (this->position_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("YPos", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->position.y_position, setter);
    }
    if (this->position_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("ZPos", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->position.z_position, setter);
    }
    if (this->profession_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Profession", state->doc->allocate_string(val.c_str()))); };
        Attribute::ProfessionFilter::to_xml_attribute(state, &this->profession_filter, setter);
    }
    if (this->render_ingame_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("IngameVisibility", state->doc->allocate_string(val.c_str()))); };
        Attribute::InvertBool::to_xml_attribute(state, &this->render_ingame, setter);
    }
    if (this->render_on_map_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MapVisibility", state->doc->allocate_string(val.c_str()))); };
        Attribute::InvertBool::to_xml_attribute(state, &this->render_on_map, setter);
    }
    if (this->render_on_minimap_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("MinimapVisibility", state->doc->allocate_string(val.c_str()))); };
        Attribute::InvertBool::to_xml_attribute(state, &this->render_on_minimap, setter);
    }
    if (this->reset_behavior_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Behavior", state->doc->allocate_string(val.c_str()))); };
        Attribute::ResetBehavior::to_xml_attribute(state, &this->reset_behavior, setter);
    }
    if (this->reset_length_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("ResetLength", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->reset_length, setter);
    }
    if (this->schedule_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Schedule", state->doc->allocate_string(val.c_str()))); };
        Attribute::String::to_xml_attribute(state, &this->schedule, setter);
    }
    if (this->schedule_duration_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("ScheduleDuration", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->schedule_duration, setter);
    }
    if (this->show_category_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Show", state->doc->allocate_string(val.c_str()))); };
        Attribute::MarkerCategory::to_xml_attribute(state, &this->show_category, setter);
    }
    if (this->specialization_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Specialization", state->doc->allocate_string(val.c_str()))); };
        Attribute::SpecializationFilter::to_xml_attribute(state, &this->specialization_filter, setter);
    }
    if (this->species_filter_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Race", state->doc->allocate_string(val.c_str()))); };
        Attribute::SpeciesFilter::to_xml_attribute(state, &this->species_filter, setter);
    }
    if (this->toggle_category_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("Toggle", state->doc->allocate_string(val.c_str()))); };
        Attribute::MarkerCategory::to_xml_attribute(state, &this->toggle_category, setter);
    }
    if (this->tooltip_description_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("TipDescription", state->doc->allocate_string(val.c_str()))); };
        Attribute::String::to_xml_attribute(state, &this->tooltip_description, setter);
    }
    if (this->tooltip_name_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("TipName", state->doc->allocate_string(val.c_str()))); };
        Attribute::String::to_xml_attribute(state, &this->tooltip_name, setter);
    }
    if (this->trigger_range_is_set) {
        std::function<void(std::string)> setter = [xml_node, state](std::string val) { xml_node->append_attribute(state->doc->allocate_attribute("TriggerRange", state->doc->allocate_string(val.c_str()))); };
        Attribute::Float::to_xml_attribute(state, &this->trigger_range, setter);
    }
    return xml_node;
}

// The following attributes are not yet supported in Burrito
// and are not written to or read from the protobuf file:
//  achievement_bit_index
//  achievement_id
//  auto_trigger
//  bounce
//  bounce_delay
//  bounce_duration
//  bounce_height
//  constant_size_on_map
//  copy_clipboard
//  copy_message
//  disable_player_cutout
//  distance_fade_end
//  distance_fade_start
//  has_countdown
//  height_offset
//  hide_category
//  icon_size
//  info_message
//  invert_visibility
//  map_display_size
//  maximum_size_on_screen
//  minimum_size_on_screen
//  reset_behavior
//  reset_length
//  schedule
//  schedule_duration
//  show_category
//  toggle_category
//  tooltip_description
//  tooltip_name
//  trigger_range

guildpoint::Icon Icon::as_protobuf(ProtoWriterState* state) const {
    guildpoint::Icon proto_icon;
    if (this->color_is_set) {
        std::function<void(uint32_t)> setter = [&proto_icon](uint32_t val) { proto_icon.set_rgba_color(val); };
        Attribute::Color::to_proto_field(this->color, state, setter);
    }
    if (this->cull_chirality_is_set) {
        std::function<void(guildpoint::CullChirality)> setter = [&proto_icon](guildpoint::CullChirality val) { proto_icon.set_cull_chirality(val); };
        Attribute::CullChirality::to_proto_field(this->cull_chirality, state, setter);
    }
    if (this->euler_rotation_is_set) {
        std::function<void(guildpoint::EulerRotation*)> setter = [&proto_icon](guildpoint::EulerRotation* val) { proto_icon.set_allocated_euler_rotation(val); };
        Attribute::EulerRotation::to_proto_field(this->euler_rotation, state, setter);
    }
    if (this->festival_filter_is_set) {
        std::function<void(guildpoint::FestivalFilter*)> setter = [&proto_icon](guildpoint::FestivalFilter* val) { proto_icon.set_allocated_festival_filter(val); };
        Attribute::FestivalFilter::to_proto_field(this->festival_filter, state, setter);
    }
    if (this->guid_is_set) {
        std::function<void(std::string)> setter = [&proto_icon](std::string val) { proto_icon.set_guid(val); };
        Attribute::UniqueId::to_proto_field(this->guid, state, setter);
    }
    if (this->icon_is_set) {
        std::function<void(unsigned int)> setter = [&proto_icon](unsigned int val) { proto_icon.set_texture_id(val); };
        Attribute::Image::to_proto_field(this->icon, state, setter);
    }
    if (this->map_id_is_set) {
        std::function<void(int)> setter = [&proto_icon](int val) { proto_icon.set_map_id(val); };
        Attribute::Int::to_proto_field(this->map_id, state, setter);
    }
    if (this->map_type_filter_is_set) {
        std::function<void(guildpoint::MapTypeFilter*)> setter = [&proto_icon](guildpoint::MapTypeFilter* val) { proto_icon.set_allocated_map_type_filter(val); };
        Attribute::MapTypeFilter::to_proto_field(this->map_type_filter, state, setter);
    }
    if (this->mount_filter_is_set) {
        std::function<void(guildpoint::MountFilter*)> setter = [&proto_icon](guildpoint::MountFilter* val) { proto_icon.set_allocated_mount_filter(val); };
        Attribute::MountFilter::to_proto_field(this->mount_filter, state, setter);
    }
    if (this->position_is_set) {
        std::function<void(guildpoint::Position*)> setter = [&proto_icon](guildpoint::Position* val) { proto_icon.set_allocated_position(val); };
        Attribute::Position::to_proto_field(this->position, state, setter);
    }
    if (this->profession_filter_is_set) {
        std::function<void(guildpoint::ProfessionFilter*)> setter = [&proto_icon](guildpoint::ProfessionFilter* val) { proto_icon.set_allocated_profession_filter(val); };
        Attribute::ProfessionFilter::to_proto_field(this->profession_filter, state, setter);
    }
    if (this->render_ingame_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_is_hidden_ingame(val); };
        Attribute::Bool::to_proto_field(this->render_ingame, state, setter);
    }
    if (this->render_on_map_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_is_hidden_on_map(val); };
        Attribute::Bool::to_proto_field(this->render_on_map, state, setter);
    }
    if (this->render_on_minimap_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_is_hidden_on_minimap(val); };
        Attribute::Bool::to_proto_field(this->render_on_minimap, state, setter);
    }
    if (this->specialization_filter_is_set) {
        std::function<void(guildpoint::SpecializationFilter*)> setter = [&proto_icon](guildpoint::SpecializationFilter* val) { proto_icon.set_allocated_specialization_filter(val); };
        Attribute::SpecializationFilter::to_proto_field(this->specialization_filter, state, setter);
    }
    if (this->species_filter_is_set) {
        std::function<void(guildpoint::SpeciesFilter*)> setter = [&proto_icon](guildpoint::SpeciesFilter* val) { proto_icon.set_allocated_species_filter(val); };
        Attribute::SpeciesFilter::to_proto_field(this->species_filter, state, setter);
    }
    return proto_icon;
}

void Icon::parse_protobuf(guildpoint::Icon proto_icon, ProtoReaderState* state) {
    if (proto_icon.rgba_color() != 0) {
        Attribute::Color::from_proto_field(proto_icon.rgba_color(), state, &(this->color), &(this->color_is_set));
    }
    if (proto_icon.cull_chirality() != 0) {
        Attribute::CullChirality::from_proto_field(proto_icon.cull_chirality(), state, &(this->cull_chirality), &(this->cull_chirality_is_set));
    }
    if (proto_icon.has_euler_rotation()) {
        Attribute::EulerRotation::from_proto_field(proto_icon.euler_rotation(), state, &(this->euler_rotation), &(this->euler_rotation_is_set));
    }
    if (proto_icon.has_festival_filter()) {
        Attribute::FestivalFilter::from_proto_field(proto_icon.festival_filter(), state, &(this->festival_filter), &(this->festival_filter_is_set));
    }
    if (proto_icon.guid() != "") {
        Attribute::UniqueId::from_proto_field(proto_icon.guid(), state, &(this->guid), &(this->guid_is_set));
    }
    if (proto_icon.texture_id() != 0) {
        Attribute::Image::from_proto_field(proto_icon.texture_id(), state, &(this->icon), &(this->icon_is_set));
    }
    if (proto_icon.map_id() != 0) {
        Attribute::Int::from_proto_field(proto_icon.map_id(), state, &(this->map_id), &(this->map_id_is_set));
    }
    if (proto_icon.has_map_type_filter()) {
        Attribute::MapTypeFilter::from_proto_field(proto_icon.map_type_filter(), state, &(this->map_type_filter), &(this->map_type_filter_is_set));
    }
    if (proto_icon.has_mount_filter()) {
        Attribute::MountFilter::from_proto_field(proto_icon.mount_filter(), state, &(this->mount_filter), &(this->mount_filter_is_set));
    }
    if (proto_icon.has_position()) {
        Attribute::Position::from_proto_field(proto_icon.position(), state, &(this->position), &(this->position_is_set));
    }
    if (proto_icon.has_profession_filter()) {
        Attribute::ProfessionFilter::from_proto_field(proto_icon.profession_filter(), state, &(this->profession_filter), &(this->profession_filter_is_set));
    }
    if (proto_icon.is_hidden_ingame() != 0) {
        Attribute::Bool::from_proto_field(proto_icon.is_hidden_ingame(), state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    if (proto_icon.is_hidden_on_map() != 0) {
        Attribute::Bool::from_proto_field(proto_icon.is_hidden_on_map(), state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    if (proto_icon.is_hidden_on_minimap() != 0) {
        Attribute::Bool::from_proto_field(proto_icon.is_hidden_on_minimap(), state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    if (proto_icon.has_specialization_filter()) {
        Attribute::SpecializationFilter::from_proto_field(proto_icon.specialization_filter(), state, &(this->specialization_filter), &(this->specialization_filter_is_set));
    }
    if (proto_icon.has_species_filter()) {
        Attribute::SpeciesFilter::from_proto_field(proto_icon.species_filter(), state, &(this->species_filter), &(this->species_filter_is_set));
    }
}
