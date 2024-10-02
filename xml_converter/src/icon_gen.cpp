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
    else if (attributename == "autotrigger") {
        xml_attribute_to_bool(attribute, errors, state, &(this->auto_trigger), &(this->auto_trigger_is_set));
    }
    else if (attributename == "bouncedelay") {
        xml_attribute_to_float(attribute, errors, state, &(this->bounce_delay), &(this->bounce_delay_is_set));
    }
    else if (attributename == "bounceduration") {
        xml_attribute_to_float(attribute, errors, state, &(this->bounce_duration), &(this->bounce_duration_is_set));
    }
    else if (attributename == "bounceheight") {
        xml_attribute_to_float(attribute, errors, state, &(this->bounce_height), &(this->bounce_height_is_set));
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
    else if (attributename == "scaleonmapwithzoom") {
        inverted_xml_attribute_to_bool(attribute, errors, state, &(this->constant_size_on_map), &(this->constant_size_on_map_is_set));
    }
    else if (attributename == "copy") {
        xml_attribute_to_string(attribute, errors, state, &(this->copy_clipboard), &(this->copy_clipboard_is_set));
    }
    else if (attributename == "copyclipboard") {
        xml_attribute_to_string(attribute, errors, state, &(this->copy_clipboard), &(this->copy_clipboard_is_set));
    }
    else if (attributename == "copymessage") {
        xml_attribute_to_string(attribute, errors, state, &(this->copy_message), &(this->copy_message_is_set));
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
    else if (attributename == "rotate") {
        xml_attribute_to_euler_rotation(attribute, errors, state, &(this->euler_rotation), &(this->euler_rotation_is_set));
    }
    else if (attributename == "rotatex") {
        xml_attribute_to_float(attribute, errors, state, &(this->euler_rotation.x_rotation), &(this->euler_rotation_is_set));
    }
    else if (attributename == "rotatey") {
        xml_attribute_to_float(attribute, errors, state, &(this->euler_rotation.y_rotation), &(this->euler_rotation_is_set));
    }
    else if (attributename == "rotatez") {
        xml_attribute_to_float(attribute, errors, state, &(this->euler_rotation.z_rotation), &(this->euler_rotation_is_set));
    }
    else if (attributename == "festival") {
        xml_attribute_to_festival_filter(attribute, errors, state, &(this->festival_filter), &(this->festival_filter_is_set));
    }
    else if (attributename == "guid") {
        xml_attribute_to_unique_id(attribute, errors, state, &(this->guid), &(this->guid_is_set));
    }
    else if (attributename == "hascountdown") {
        xml_attribute_to_bool(attribute, errors, state, &(this->has_countdown), &(this->has_countdown_is_set));
    }
    else if (attributename == "heightoffset") {
        xml_attribute_to_float(attribute, errors, state, &(this->height_offset), &(this->height_offset_is_set));
    }
    else if (attributename == "bhheightoffset") {
        xml_attribute_to_float(attribute, errors, state, &(this->height_offset), &(this->height_offset_is_set));
    }
    else if (attributename == "hide") {
        xml_attribute_to_marker_category(attribute, errors, state, &(this->hide_category), &(this->hide_category_is_set));
    }
    else if (attributename == "iconfile") {
        xml_attribute_to_image(attribute, errors, state, &(this->icon), &(this->icon_is_set));
    }
    else if (attributename == "iconsize") {
        xml_attribute_to_float(attribute, errors, state, &(this->icon_size), &(this->icon_size_is_set));
    }
    else if (attributename == "info") {
        xml_attribute_to_string(attribute, errors, state, &(this->info_message), &(this->info_message_is_set));
    }
    else if (attributename == "invertbehavior") {
        xml_attribute_to_bool(attribute, errors, state, &(this->invert_visibility), &(this->invert_visibility_is_set));
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
    else if (attributename == "maxsize") {
        xml_attribute_to_int(attribute, errors, state, &(this->maximum_size_on_screen), &(this->maximum_size_on_screen_is_set));
    }
    else if (attributename == "minsize") {
        xml_attribute_to_int(attribute, errors, state, &(this->minimum_size_on_screen), &(this->minimum_size_on_screen_is_set));
    }
    else if (attributename == "mount") {
        xml_attribute_to_mount_filter(attribute, errors, state, &(this->mount_filter), &(this->mount_filter_is_set));
    }
    else if (attributename == "position") {
        xml_attribute_to_position(attribute, errors, state, &(this->position), &(this->position_is_set));
    }
    else if (attributename == "xpos") {
        xml_attribute_to_float(attribute, errors, state, &(this->position.x_position), &(this->position_is_set));
    }
    else if (attributename == "positionx") {
        xml_attribute_to_float(attribute, errors, state, &(this->position.x_position), &(this->position_is_set));
    }
    else if (attributename == "ypos") {
        xml_attribute_to_float(attribute, errors, state, &(this->position.y_position), &(this->position_is_set));
    }
    else if (attributename == "positiony") {
        xml_attribute_to_float(attribute, errors, state, &(this->position.y_position), &(this->position_is_set));
    }
    else if (attributename == "zpos") {
        xml_attribute_to_float(attribute, errors, state, &(this->position.z_position), &(this->position_is_set));
    }
    else if (attributename == "positionz") {
        xml_attribute_to_float(attribute, errors, state, &(this->position.z_position), &(this->position_is_set));
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
    else if (attributename == "behavior") {
        xml_attribute_to_reset_behavior(attribute, errors, state, &(this->reset_behavior), &(this->reset_behavior_is_set));
    }
    else if (attributename == "resetlength") {
        xml_attribute_to_float(attribute, errors, state, &(this->reset_length), &(this->reset_length_is_set));
    }
    else if (attributename == "schedule") {
        xml_attribute_to_string(attribute, errors, state, &(this->schedule), &(this->schedule_is_set));
    }
    else if (attributename == "scheduleduration") {
        xml_attribute_to_float(attribute, errors, state, &(this->schedule_duration), &(this->schedule_duration_is_set));
    }
    else if (attributename == "show") {
        xml_attribute_to_marker_category(attribute, errors, state, &(this->show_category), &(this->show_category_is_set));
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
    else if (attributename == "toggle") {
        xml_attribute_to_marker_category(attribute, errors, state, &(this->toggle_category), &(this->toggle_category_is_set));
    }
    else if (attributename == "togglecategory") {
        xml_attribute_to_marker_category(attribute, errors, state, &(this->toggle_category), &(this->toggle_category_is_set));
    }
    else if (attributename == "tipdescription") {
        xml_attribute_to_string(attribute, errors, state, &(this->tooltip_description), &(this->tooltip_description_is_set));
    }
    else if (attributename == "tipname") {
        xml_attribute_to_string(attribute, errors, state, &(this->tooltip_name), &(this->tooltip_name_is_set));
    }
    else if (attributename == "triggerrange") {
        xml_attribute_to_float(attribute, errors, state, &(this->trigger_range), &(this->trigger_range_is_set));
    }
    else if (attributename == "inforange") {
        xml_attribute_to_float(attribute, errors, state, &(this->trigger_range), &(this->trigger_range_is_set));
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

vector<string> Icon::as_xml(XMLWriterState* state) const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<POI ");
    if (this->achievement_bit_index_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("AchievementBit", state, &this->achievement_bit_index));
    }
    if (this->achievement_id_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("AchievementId", state, &this->achievement_id));
    }
    if (this->auto_trigger_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("AutoTrigger", state, &this->auto_trigger));
    }
    if (this->bounce_delay_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("BounceDelay", state, &this->bounce_delay));
    }
    if (this->bounce_duration_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("BounceDuration", state, &this->bounce_duration));
    }
    if (this->bounce_height_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("BounceHeight", state, &this->bounce_height));
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
    if (this->constant_size_on_map_is_set) {
        xml_node_contents.push_back(bool_to_inverted_xml_attribute("ScaleOnMapWithZoom", state, &this->constant_size_on_map));
    }
    if (this->copy_clipboard_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("Copy", state, &this->copy_clipboard));
    }
    if (this->copy_message_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("CopyMessage", state, &this->copy_message));
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
    if (this->has_countdown_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("HasCountdown", state, &this->has_countdown));
    }
    if (this->height_offset_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("HeightOffset", state, &this->height_offset));
    }
    if (this->hide_category_is_set) {
        xml_node_contents.push_back(marker_category_to_xml_attribute("Hide", state, &this->hide_category));
    }
    if (this->icon_is_set) {
        xml_node_contents.push_back(image_to_xml_attribute("IconFile", state, &this->icon));
    }
    if (this->icon_size_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("IconSize", state, &this->icon_size));
    }
    if (this->info_message_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("Info", state, &this->info_message));
    }
    if (this->invert_visibility_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("InvertBehavior", state, &this->invert_visibility));
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
    if (this->maximum_size_on_screen_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("MaxSize", state, &this->maximum_size_on_screen));
    }
    if (this->minimum_size_on_screen_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("MinSize", state, &this->minimum_size_on_screen));
    }
    if (this->mount_filter_is_set) {
        xml_node_contents.push_back(mount_filter_to_xml_attribute("Mount", state, &this->mount_filter));
    }
    if (this->position_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("XPos", state, &this->position.x_position));
    }
    if (this->position_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("YPos", state, &this->position.y_position));
    }
    if (this->position_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("ZPos", state, &this->position.z_position));
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
    if (this->reset_behavior_is_set) {
        xml_node_contents.push_back(reset_behavior_to_xml_attribute("Behavior", state, &this->reset_behavior));
    }
    if (this->reset_length_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("ResetLength", state, &this->reset_length));
    }
    if (this->schedule_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("Schedule", state, &this->schedule));
    }
    if (this->schedule_duration_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("ScheduleDuration", state, &this->schedule_duration));
    }
    if (this->show_category_is_set) {
        xml_node_contents.push_back(marker_category_to_xml_attribute("Show", state, &this->show_category));
    }
    if (this->specialization_filter_is_set) {
        xml_node_contents.push_back(specialization_filter_to_xml_attribute("Specialization", state, &this->specialization_filter));
    }
    if (this->species_filter_is_set) {
        xml_node_contents.push_back(species_filter_to_xml_attribute("Race", state, &this->species_filter));
    }
    if (this->toggle_category_is_set) {
        xml_node_contents.push_back(marker_category_to_xml_attribute("Toggle", state, &this->toggle_category));
    }
    if (this->tooltip_description_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("TipDescription", state, &this->tooltip_description));
    }
    if (this->tooltip_name_is_set) {
        xml_node_contents.push_back(string_to_xml_attribute("TipName", state, &this->tooltip_name));
    }
    if (this->trigger_range_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("TriggerRange", state, &this->trigger_range));
    }
    xml_node_contents.push_back("/>");
    return xml_node_contents;
}

guildpoint::Icon Icon::as_protobuf(ProtoWriterState* state) const {
    guildpoint::Icon proto_icon;
    if (this->achievement_bit_index_is_set) {
        std::function<void(int)> setter = [&proto_icon](int val) { proto_icon.set_achievement_bit_index(val); };
        int_to_proto(this->achievement_bit_index, state, setter);
    }
    if (this->achievement_id_is_set) {
        std::function<void(int)> setter = [&proto_icon](int val) { proto_icon.set_achievement_id(val); };
        int_to_proto(this->achievement_id, state, setter);
    }
    if (this->auto_trigger_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.mutable_trigger()->set_auto_trigger(val); };
        bool_to_proto(this->auto_trigger, state, setter);
    }
    if (this->bounce_delay_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.mutable_trigger()->set_bounce_delay(val); };
        float_to_proto(this->bounce_delay, state, setter);
    }
    if (this->bounce_duration_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.mutable_trigger()->set_bounce_duration(val); };
        float_to_proto(this->bounce_duration, state, setter);
    }
    if (this->bounce_height_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.mutable_trigger()->set_bounce_height(val); };
        float_to_proto(this->bounce_height, state, setter);
    }
    if (this->color_is_set) {
        std::function<void(uint32_t)> setter = [&proto_icon](uint32_t val) { proto_icon.set_rgba_color(val); };
        color_to_proto(this->color, state, setter);
    }
    if (this->constant_size_on_map_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_constant_size_on_map(val); };
        bool_to_proto(this->constant_size_on_map, state, setter);
    }
    if (this->copy_clipboard_is_set) {
        std::function<void(std::string)> setter = [&proto_icon](std::string val) { proto_icon.mutable_trigger()->set_action_copy_clipboard(val); };
        string_to_proto(this->copy_clipboard, state, setter);
    }
    if (this->copy_message_is_set) {
        std::function<void(std::string)> setter = [&proto_icon](std::string val) { proto_icon.mutable_trigger()->set_action_copy_message(val); };
        string_to_proto(this->copy_message, state, setter);
    }
    if (this->cull_chirality_is_set) {
        std::function<void(guildpoint::CullChirality)> setter = [&proto_icon](guildpoint::CullChirality val) { proto_icon.set_cull_chirality(val); };
        cull_chirality_to_proto(this->cull_chirality, state, setter);
    }
    if (this->disable_player_cutout_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_disable_player_cutout(val); };
        bool_to_proto(this->disable_player_cutout, state, setter);
    }
    if (this->distance_fade_end_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.set_distance_fade_end(val); };
        float_to_proto(this->distance_fade_end, state, setter);
    }
    if (this->distance_fade_start_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.set_distance_fade_start(val); };
        float_to_proto(this->distance_fade_start, state, setter);
    }
    if (this->euler_rotation_is_set) {
        std::function<void(guildpoint::EulerRotation*)> setter = [&proto_icon](guildpoint::EulerRotation* val) { proto_icon.set_allocated_euler_rotation(val); };
        euler_rotation_to_proto(this->euler_rotation, state, setter);
    }
    if (this->festival_filter_is_set) {
        std::function<void(guildpoint::FestivalFilter*)> setter = [&proto_icon](guildpoint::FestivalFilter* val) { proto_icon.set_allocated_festival_filter(val); };
        festival_filter_to_proto(this->festival_filter, state, setter);
    }
    if (this->guid_is_set) {
        std::function<void(std::string)> setter = [&proto_icon](std::string val) { proto_icon.set_guid(val); };
        unique_id_to_proto(this->guid, state, setter);
    }
    if (this->has_countdown_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.mutable_trigger()->set_has_countdown(val); };
        bool_to_proto(this->has_countdown, state, setter);
    }
    if (this->height_offset_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.set_height_offset(val); };
        float_to_proto(this->height_offset, state, setter);
    }
    if (this->hide_category_is_set) {
        std::function<void(guildpoint::Category*)> setter = [&proto_icon](guildpoint::Category* val) { proto_icon.mutable_trigger()->set_allocated_action_hide_category(val); };
        marker_category_to_proto(this->hide_category, state, setter);
    }
    if (this->icon_is_set) {
        std::function<void(unsigned int)> setter = [&proto_icon](unsigned int val) { proto_icon.set_texture_id(val); };
        image_to_proto(this->icon, state, setter);
    }
    if (this->icon_size_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.set_tentative__scale(val); };
        float_to_proto(this->icon_size, state, setter);
    }
    if (this->info_message_is_set) {
        std::function<void(std::string)> setter = [&proto_icon](std::string val) { proto_icon.mutable_trigger()->set_action_info_message(val); };
        string_to_proto(this->info_message, state, setter);
    }
    if (this->invert_visibility_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.mutable_trigger()->set_invert_display(val); };
        bool_to_proto(this->invert_visibility, state, setter);
    }
    if (this->map_display_size_is_set) {
        std::function<void(int)> setter = [&proto_icon](int val) { proto_icon.set_map_display_size(val); };
        int_to_proto(this->map_display_size, state, setter);
    }
    if (this->map_id_is_set) {
        std::function<void(int)> setter = [&proto_icon](int val) { proto_icon.set_map_id(val); };
        int_to_proto(this->map_id, state, setter);
    }
    if (this->map_type_filter_is_set) {
        std::function<void(guildpoint::MapTypeFilter*)> setter = [&proto_icon](guildpoint::MapTypeFilter* val) { proto_icon.set_allocated_map_type_filter(val); };
        map_type_filter_to_proto(this->map_type_filter, state, setter);
    }
    if (this->maximum_size_on_screen_is_set) {
        std::function<void(int)> setter = [&proto_icon](int val) { proto_icon.set_maximum_size_on_screen(val); };
        int_to_proto(this->maximum_size_on_screen, state, setter);
    }
    if (this->minimum_size_on_screen_is_set) {
        std::function<void(int)> setter = [&proto_icon](int val) { proto_icon.set_minimum_size_on_screen(val); };
        int_to_proto(this->minimum_size_on_screen, state, setter);
    }
    if (this->mount_filter_is_set) {
        std::function<void(guildpoint::MountFilter*)> setter = [&proto_icon](guildpoint::MountFilter* val) { proto_icon.set_allocated_mount_filter(val); };
        mount_filter_to_proto(this->mount_filter, state, setter);
    }
    if (this->position_is_set) {
        std::function<void(guildpoint::Position*)> setter = [&proto_icon](guildpoint::Position* val) { proto_icon.set_allocated_position(val); };
        position_to_proto(this->position, state, setter);
    }
    if (this->profession_filter_is_set) {
        std::function<void(guildpoint::ProfessionFilter*)> setter = [&proto_icon](guildpoint::ProfessionFilter* val) { proto_icon.set_allocated_profession_filter(val); };
        profession_filter_to_proto(this->profession_filter, state, setter);
    }
    if (this->render_ingame_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_is_hidden_ingame(val); };
        bool_to_proto(this->render_ingame, state, setter);
    }
    if (this->render_on_map_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_is_hidden_on_map(val); };
        bool_to_proto(this->render_on_map, state, setter);
    }
    if (this->render_on_minimap_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_is_hidden_on_minimap(val); };
        bool_to_proto(this->render_on_minimap, state, setter);
    }
    if (this->reset_behavior_is_set) {
        std::function<void(guildpoint::ResetBehavior)> setter = [&proto_icon](guildpoint::ResetBehavior val) { proto_icon.mutable_trigger()->set_reset_behavior(val); };
        reset_behavior_to_proto(this->reset_behavior, state, setter);
    }
    if (this->reset_length_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.mutable_trigger()->set_reset_length(val); };
        float_to_proto(this->reset_length, state, setter);
    }
    if (this->schedule_is_set) {
        std::function<void(std::string)> setter = [&proto_icon](std::string val) { proto_icon.set_bhdraft__schedule(val); };
        string_to_proto(this->schedule, state, setter);
    }
    if (this->schedule_duration_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.set_bhdraft__schedule_duration(val); };
        float_to_proto(this->schedule_duration, state, setter);
    }
    if (this->show_category_is_set) {
        std::function<void(guildpoint::Category*)> setter = [&proto_icon](guildpoint::Category* val) { proto_icon.mutable_trigger()->set_allocated_action_show_category(val); };
        marker_category_to_proto(this->show_category, state, setter);
    }
    if (this->specialization_filter_is_set) {
        std::function<void(guildpoint::SpecializationFilter*)> setter = [&proto_icon](guildpoint::SpecializationFilter* val) { proto_icon.set_allocated_specialization_filter(val); };
        specialization_filter_to_proto(this->specialization_filter, state, setter);
    }
    if (this->species_filter_is_set) {
        std::function<void(guildpoint::SpeciesFilter*)> setter = [&proto_icon](guildpoint::SpeciesFilter* val) { proto_icon.set_allocated_species_filter(val); };
        species_filter_to_proto(this->species_filter, state, setter);
    }
    if (this->toggle_category_is_set) {
        std::function<void(guildpoint::Category*)> setter = [&proto_icon](guildpoint::Category* val) { proto_icon.mutable_trigger()->set_allocated_action_toggle_category(val); };
        marker_category_to_proto(this->toggle_category, state, setter);
    }
    if (this->tooltip_description_is_set) {
        std::function<void(std::string)> setter = [&proto_icon](std::string val) { proto_icon.set_tip_description(val); };
        string_to_proto(this->tooltip_description, state, setter);
    }
    if (this->tooltip_name_is_set) {
        std::function<void(std::string)> setter = [&proto_icon](std::string val) { proto_icon.set_tip_name(val); };
        string_to_proto(this->tooltip_name, state, setter);
    }
    if (this->trigger_range_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.mutable_trigger()->set_range(val); };
        float_to_proto(this->trigger_range, state, setter);
    }
    return proto_icon;
}

void Icon::parse_protobuf(guildpoint::Icon proto_icon, ProtoReaderState* state) {
    if (proto_icon.achievement_bit_index() != 0) {
        proto_to_int(proto_icon.achievement_bit_index(), state, &(this->achievement_bit_index), &(this->achievement_bit_index_is_set));
    }
    if (proto_icon.achievement_id() != 0) {
        proto_to_int(proto_icon.achievement_id(), state, &(this->achievement_id), &(this->achievement_id_is_set));
    }
    if (proto_icon.trigger().auto_trigger() != 0) {
        proto_to_bool(proto_icon.trigger().auto_trigger(), state, &(this->auto_trigger), &(this->auto_trigger_is_set));
    }
    if (proto_icon.trigger().bounce_delay() != 0) {
        proto_to_float(proto_icon.trigger().bounce_delay(), state, &(this->bounce_delay), &(this->bounce_delay_is_set));
    }
    if (proto_icon.trigger().bounce_duration() != 0) {
        proto_to_float(proto_icon.trigger().bounce_duration(), state, &(this->bounce_duration), &(this->bounce_duration_is_set));
    }
    if (proto_icon.trigger().bounce_height() != 0) {
        proto_to_float(proto_icon.trigger().bounce_height(), state, &(this->bounce_height), &(this->bounce_height_is_set));
    }
    if (proto_icon.rgba_color() != 0) {
        proto_to_color(proto_icon.rgba_color(), state, &(this->color), &(this->color_is_set));
    }
    if (proto_icon.constant_size_on_map() != 0) {
        proto_to_bool(proto_icon.constant_size_on_map(), state, &(this->constant_size_on_map), &(this->constant_size_on_map_is_set));
    }
    if (proto_icon.trigger().action_copy_clipboard() != "") {
        proto_to_string(proto_icon.trigger().action_copy_clipboard(), state, &(this->copy_clipboard), &(this->copy_clipboard_is_set));
    }
    if (proto_icon.trigger().action_copy_message() != "") {
        proto_to_string(proto_icon.trigger().action_copy_message(), state, &(this->copy_message), &(this->copy_message_is_set));
    }
    if (proto_icon.cull_chirality() != 0) {
        proto_to_cull_chirality(proto_icon.cull_chirality(), state, &(this->cull_chirality), &(this->cull_chirality_is_set));
    }
    if (proto_icon.disable_player_cutout() != 0) {
        proto_to_bool(proto_icon.disable_player_cutout(), state, &(this->disable_player_cutout), &(this->disable_player_cutout_is_set));
    }
    if (proto_icon.distance_fade_end() != 0) {
        proto_to_float(proto_icon.distance_fade_end(), state, &(this->distance_fade_end), &(this->distance_fade_end_is_set));
    }
    if (proto_icon.distance_fade_start() != 0) {
        proto_to_float(proto_icon.distance_fade_start(), state, &(this->distance_fade_start), &(this->distance_fade_start_is_set));
    }
    if (proto_icon.has_euler_rotation()) {
        proto_to_euler_rotation(proto_icon.euler_rotation(), state, &(this->euler_rotation), &(this->euler_rotation_is_set));
    }
    if (proto_icon.has_festival_filter()) {
        proto_to_festival_filter(proto_icon.festival_filter(), state, &(this->festival_filter), &(this->festival_filter_is_set));
    }
    if (proto_icon.guid() != "") {
        proto_to_unique_id(proto_icon.guid(), state, &(this->guid), &(this->guid_is_set));
    }
    if (proto_icon.trigger().has_countdown() != 0) {
        proto_to_bool(proto_icon.trigger().has_countdown(), state, &(this->has_countdown), &(this->has_countdown_is_set));
    }
    if (proto_icon.height_offset() != 0) {
        proto_to_float(proto_icon.height_offset(), state, &(this->height_offset), &(this->height_offset_is_set));
    }
    if (proto_icon.trigger().has_action_hide_category()) {
        proto_to_marker_category(proto_icon.trigger().action_hide_category(), state, &(this->hide_category), &(this->hide_category_is_set));
    }
    if (proto_icon.texture_id() != 0) {
        proto_to_image(proto_icon.texture_id(), state, &(this->icon), &(this->icon_is_set));
    }
    if (proto_icon.tentative__scale() != 0) {
        proto_to_float(proto_icon.tentative__scale(), state, &(this->icon_size), &(this->icon_size_is_set));
    }
    if (proto_icon.trigger().action_info_message() != "") {
        proto_to_string(proto_icon.trigger().action_info_message(), state, &(this->info_message), &(this->info_message_is_set));
    }
    if (proto_icon.trigger().invert_display() != 0) {
        proto_to_bool(proto_icon.trigger().invert_display(), state, &(this->invert_visibility), &(this->invert_visibility_is_set));
    }
    if (proto_icon.map_display_size() != 0) {
        proto_to_int(proto_icon.map_display_size(), state, &(this->map_display_size), &(this->map_display_size_is_set));
    }
    if (proto_icon.map_id() != 0) {
        proto_to_int(proto_icon.map_id(), state, &(this->map_id), &(this->map_id_is_set));
    }
    if (proto_icon.has_map_type_filter()) {
        proto_to_map_type_filter(proto_icon.map_type_filter(), state, &(this->map_type_filter), &(this->map_type_filter_is_set));
    }
    if (proto_icon.maximum_size_on_screen() != 0) {
        proto_to_int(proto_icon.maximum_size_on_screen(), state, &(this->maximum_size_on_screen), &(this->maximum_size_on_screen_is_set));
    }
    if (proto_icon.minimum_size_on_screen() != 0) {
        proto_to_int(proto_icon.minimum_size_on_screen(), state, &(this->minimum_size_on_screen), &(this->minimum_size_on_screen_is_set));
    }
    if (proto_icon.has_mount_filter()) {
        proto_to_mount_filter(proto_icon.mount_filter(), state, &(this->mount_filter), &(this->mount_filter_is_set));
    }
    if (proto_icon.has_position()) {
        proto_to_position(proto_icon.position(), state, &(this->position), &(this->position_is_set));
    }
    if (proto_icon.has_profession_filter()) {
        proto_to_profession_filter(proto_icon.profession_filter(), state, &(this->profession_filter), &(this->profession_filter_is_set));
    }
    if (proto_icon.is_hidden_ingame() != 0) {
        proto_to_bool(proto_icon.is_hidden_ingame(), state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    if (proto_icon.is_hidden_on_map() != 0) {
        proto_to_bool(proto_icon.is_hidden_on_map(), state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    if (proto_icon.is_hidden_on_minimap() != 0) {
        proto_to_bool(proto_icon.is_hidden_on_minimap(), state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    if (proto_icon.trigger().reset_behavior() != 0) {
        proto_to_reset_behavior(proto_icon.trigger().reset_behavior(), state, &(this->reset_behavior), &(this->reset_behavior_is_set));
    }
    if (proto_icon.trigger().reset_length() != 0) {
        proto_to_float(proto_icon.trigger().reset_length(), state, &(this->reset_length), &(this->reset_length_is_set));
    }
    if (proto_icon.bhdraft__schedule() != "") {
        proto_to_string(proto_icon.bhdraft__schedule(), state, &(this->schedule), &(this->schedule_is_set));
    }
    if (proto_icon.bhdraft__schedule_duration() != 0) {
        proto_to_float(proto_icon.bhdraft__schedule_duration(), state, &(this->schedule_duration), &(this->schedule_duration_is_set));
    }
    if (proto_icon.trigger().has_action_show_category()) {
        proto_to_marker_category(proto_icon.trigger().action_show_category(), state, &(this->show_category), &(this->show_category_is_set));
    }
    if (proto_icon.has_specialization_filter()) {
        proto_to_specialization_filter(proto_icon.specialization_filter(), state, &(this->specialization_filter), &(this->specialization_filter_is_set));
    }
    if (proto_icon.has_species_filter()) {
        proto_to_species_filter(proto_icon.species_filter(), state, &(this->species_filter), &(this->species_filter_is_set));
    }
    if (proto_icon.trigger().has_action_toggle_category()) {
        proto_to_marker_category(proto_icon.trigger().action_toggle_category(), state, &(this->toggle_category), &(this->toggle_category_is_set));
    }
    if (proto_icon.tip_description() != "") {
        proto_to_string(proto_icon.tip_description(), state, &(this->tooltip_description), &(this->tooltip_description_is_set));
    }
    if (proto_icon.tip_name() != "") {
        proto_to_string(proto_icon.tip_name(), state, &(this->tooltip_name), &(this->tooltip_name_is_set));
    }
    if (proto_icon.trigger().range() != 0) {
        proto_to_float(proto_icon.trigger().range(), state, &(this->trigger_range), &(this->trigger_range_is_set));
    }
}
