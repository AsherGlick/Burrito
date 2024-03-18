#include "icon_gen.hpp"

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

string Icon::classname() {
    return "POI";
}

bool Icon::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors, XMLReaderState* state) {
    string attributename;
    attributename = normalize(get_attribute_name(attribute));
    if (attributename == "achievementbit") {
        xml_attribute_to_int(attribute, errors, state, &(this->achievement_bitmask), &(this->achievement_bitmask_is_set));
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
    else if (attributename == "behavior") {
        xml_attribute_to_reset_behavior(attribute, errors, state, &(this->reset_behavior), &(this->reset_behavior_is_set));
    }
    else if (attributename == "resetlength") {
        xml_attribute_to_float(attribute, errors, state, &(this->reset_length), &(this->reset_length_is_set));
    }
    else if (attributename == "scaleonmapwithzoom") {
        xml_attribute_to_bool(attribute, errors, state, &(this->scale_on_map_with_zoom), &(this->scale_on_map_with_zoom_is_set));
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
    if (this->achievement_bitmask_is_set) {
        xml_node_contents.push_back(int_to_xml_attribute("AchievementBit", state, &this->achievement_bitmask));
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
        xml_node_contents.push_back(bool_to_xml_attribute("IngameVisibility", state, &this->render_ingame));
    }
    if (this->render_on_map_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("MapVisibility", state, &this->render_on_map));
    }
    if (this->render_on_minimap_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("MinimapVisibility", state, &this->render_on_minimap));
    }
    if (this->reset_behavior_is_set) {
        xml_node_contents.push_back(reset_behavior_to_xml_attribute("Behavior", state, &this->reset_behavior));
    }
    if (this->reset_length_is_set) {
        xml_node_contents.push_back(float_to_xml_attribute("ResetLength", state, &this->reset_length));
    }
    if (this->scale_on_map_with_zoom_is_set) {
        xml_node_contents.push_back(bool_to_xml_attribute("ScaleOnMapWithZoom", state, &this->scale_on_map_with_zoom));
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

waypoint::Icon Icon::as_protobuf(ProtoWriterState* state) const {
    waypoint::Icon proto_icon;
    if (this->achievement_bitmask_is_set) {
        std::function<void(int)> setter = [&proto_icon](int val) { proto_icon.set_achievement_bit(val); };
        int_to_proto(this->achievement_bitmask, state, setter);
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
    if (this->category_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_category(val); };
        do_nothing(this->category, state, setter);
    }
    if (this->color_is_set) {
        std::function<void(waypoint::RGBAColor*)> setter = [&proto_icon](waypoint::RGBAColor* val) { proto_icon.set_allocated_rgba_color(val); };
        color_to_proto(this->color, state, setter);
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
        std::function<void(waypoint::CullChirality)> setter = [&proto_icon](waypoint::CullChirality val) { proto_icon.set_cull_chirality(val); };
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
        std::function<void(waypoint::EulerRotation*)> setter = [&proto_icon](waypoint::EulerRotation* val) { proto_icon.set_allocated_euler_rotation(val); };
        euler_rotation_to_proto(this->euler_rotation, state, setter);
    }
    if (this->festival_filter_is_set) {
        std::function<void(waypoint::FestivalFilter*)> setter = [&proto_icon](waypoint::FestivalFilter* val) { proto_icon.set_allocated_festival_filter(val); };
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
        std::function<void(waypoint::Category*)> setter = [&proto_icon](waypoint::Category* val) { proto_icon.mutable_trigger()->set_allocated_action_hide_category(val); };
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
        std::function<void(waypoint::MapTypeFilter*)> setter = [&proto_icon](waypoint::MapTypeFilter* val) { proto_icon.set_allocated_map_type_filter(val); };
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
        std::function<void(waypoint::MountFilter*)> setter = [&proto_icon](waypoint::MountFilter* val) { proto_icon.set_allocated_mount_filter(val); };
        mount_filter_to_proto(this->mount_filter, state, setter);
    }
    if (this->position_is_set) {
        std::function<void(waypoint::Position*)> setter = [&proto_icon](waypoint::Position* val) { proto_icon.set_allocated_position(val); };
        position_to_proto(this->position, state, setter);
    }
    if (this->profession_filter_is_set) {
        std::function<void(waypoint::ProfessionFilter*)> setter = [&proto_icon](waypoint::ProfessionFilter* val) { proto_icon.set_allocated_profession_filter(val); };
        profession_filter_to_proto(this->profession_filter, state, setter);
    }
    if (this->render_ingame_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_tentative__render_ingame(val); };
        bool_to_proto(this->render_ingame, state, setter);
    }
    if (this->render_on_map_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_tentative__render_on_map(val); };
        bool_to_proto(this->render_on_map, state, setter);
    }
    if (this->render_on_minimap_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_tentative__render_on_minimap(val); };
        bool_to_proto(this->render_on_minimap, state, setter);
    }
    if (this->reset_behavior_is_set) {
        std::function<void(waypoint::ResetBehavior)> setter = [&proto_icon](waypoint::ResetBehavior val) { proto_icon.mutable_trigger()->set_reset_behavior(val); };
        reset_behavior_to_proto(this->reset_behavior, state, setter);
    }
    if (this->reset_length_is_set) {
        std::function<void(float)> setter = [&proto_icon](float val) { proto_icon.mutable_trigger()->set_reset_length(val); };
        float_to_proto(this->reset_length, state, setter);
    }
    if (this->scale_on_map_with_zoom_is_set) {
        std::function<void(bool)> setter = [&proto_icon](bool val) { proto_icon.set_scale_on_map_with_zoom(val); };
        bool_to_proto(this->scale_on_map_with_zoom, state, setter);
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
        std::function<void(waypoint::Category*)> setter = [&proto_icon](waypoint::Category* val) { proto_icon.mutable_trigger()->set_allocated_action_show_category(val); };
        marker_category_to_proto(this->show_category, state, setter);
    }
    if (this->specialization_filter_is_set) {
        std::function<void(waypoint::SpecializationFilter*)> setter = [&proto_icon](waypoint::SpecializationFilter* val) { proto_icon.set_allocated_specialization_filter(val); };
        specialization_filter_to_proto(this->specialization_filter, state, setter);
    }
    if (this->species_filter_is_set) {
        std::function<void(waypoint::SpeciesFilter*)> setter = [&proto_icon](waypoint::SpeciesFilter* val) { proto_icon.set_allocated_species_filter(val); };
        species_filter_to_proto(this->species_filter, state, setter);
    }
    if (this->toggle_category_is_set) {
        std::function<void(waypoint::Category*)> setter = [&proto_icon](waypoint::Category* val) { proto_icon.mutable_trigger()->set_allocated_action_toggle_category(val); };
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

void Icon::parse_protobuf(waypoint::Icon proto_icon, ProtoReaderState* state) {
    if (proto_icon.achievement_bit() != 0) {
        proto_to_int(proto_icon.achievement_bit(), state, &(this->achievement_bitmask), &(this->achievement_bitmask_is_set));
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
    if (proto_icon.category() != 0) {
        do_nothing(proto_icon.category(), state, &(this->category), &(this->category_is_set));
    }
    if (proto_icon.has_rgba_color()) {
        proto_to_color(proto_icon.rgba_color(), state, &(this->color), &(this->color_is_set));
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
    if (proto_icon.tentative__render_ingame() != 0) {
        proto_to_bool(proto_icon.tentative__render_ingame(), state, &(this->render_ingame), &(this->render_ingame_is_set));
    }
    if (proto_icon.tentative__render_on_map() != 0) {
        proto_to_bool(proto_icon.tentative__render_on_map(), state, &(this->render_on_map), &(this->render_on_map_is_set));
    }
    if (proto_icon.tentative__render_on_minimap() != 0) {
        proto_to_bool(proto_icon.tentative__render_on_minimap(), state, &(this->render_on_minimap), &(this->render_on_minimap_is_set));
    }
    if (proto_icon.trigger().reset_behavior() != 0) {
        proto_to_reset_behavior(proto_icon.trigger().reset_behavior(), state, &(this->reset_behavior), &(this->reset_behavior_is_set));
    }
    if (proto_icon.trigger().reset_length() != 0) {
        proto_to_float(proto_icon.trigger().reset_length(), state, &(this->reset_length), &(this->reset_length_is_set));
    }
    if (proto_icon.scale_on_map_with_zoom() != 0) {
        proto_to_bool(proto_icon.scale_on_map_with_zoom(), state, &(this->scale_on_map_with_zoom), &(this->scale_on_map_with_zoom_is_set));
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

////////////////////////////////////////////////////////////////////////////////
// apply_underlay
//
// Transforms this Icon as if this class was overlayed on top of the
// underlay argument.
////////////////////////////////////////////////////////////////////////////////
void Icon::apply_underlay(const Icon& underlay) {
    if (!this->achievement_bitmask_is_set && underlay.achievement_bitmask_is_set) {
        this->achievement_bitmask = underlay.achievement_bitmask;
        this->achievement_bitmask_is_set = true;
    }
    if (!this->achievement_id_is_set && underlay.achievement_id_is_set) {
        this->achievement_id = underlay.achievement_id;
        this->achievement_id_is_set = true;
    }
    if (!this->auto_trigger_is_set && underlay.auto_trigger_is_set) {
        this->auto_trigger = underlay.auto_trigger;
        this->auto_trigger_is_set = true;
    }
    if (!this->bounce_delay_is_set && underlay.bounce_delay_is_set) {
        this->bounce_delay = underlay.bounce_delay;
        this->bounce_delay_is_set = true;
    }
    if (!this->bounce_duration_is_set && underlay.bounce_duration_is_set) {
        this->bounce_duration = underlay.bounce_duration;
        this->bounce_duration_is_set = true;
    }
    if (!this->bounce_height_is_set && underlay.bounce_height_is_set) {
        this->bounce_height = underlay.bounce_height;
        this->bounce_height_is_set = true;
    }
    if (!this->category_is_set && underlay.category_is_set) {
        this->category = underlay.category;
        this->category_is_set = true;
    }
    if (!this->color_is_set && underlay.color_is_set) {
        this->color = underlay.color;
        this->color_is_set = true;
    }
    if (!this->copy_clipboard_is_set && underlay.copy_clipboard_is_set) {
        this->copy_clipboard = underlay.copy_clipboard;
        this->copy_clipboard_is_set = true;
    }
    if (!this->copy_message_is_set && underlay.copy_message_is_set) {
        this->copy_message = underlay.copy_message;
        this->copy_message_is_set = true;
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
    if (!this->euler_rotation_is_set && underlay.euler_rotation_is_set) {
        this->euler_rotation = underlay.euler_rotation;
        this->euler_rotation_is_set = true;
    }
    if (!this->festival_filter_is_set && underlay.festival_filter_is_set) {
        this->festival_filter = underlay.festival_filter;
        this->festival_filter_is_set = true;
    }
    if (!this->guid_is_set && underlay.guid_is_set) {
        this->guid = underlay.guid;
        this->guid_is_set = true;
    }
    if (!this->has_countdown_is_set && underlay.has_countdown_is_set) {
        this->has_countdown = underlay.has_countdown;
        this->has_countdown_is_set = true;
    }
    if (!this->height_offset_is_set && underlay.height_offset_is_set) {
        this->height_offset = underlay.height_offset;
        this->height_offset_is_set = true;
    }
    if (!this->hide_category_is_set && underlay.hide_category_is_set) {
        this->hide_category = underlay.hide_category;
        this->hide_category_is_set = true;
    }
    if (!this->icon_is_set && underlay.icon_is_set) {
        this->icon = underlay.icon;
        this->icon_is_set = true;
    }
    if (!this->icon_size_is_set && underlay.icon_size_is_set) {
        this->icon_size = underlay.icon_size;
        this->icon_size_is_set = true;
    }
    if (!this->info_message_is_set && underlay.info_message_is_set) {
        this->info_message = underlay.info_message;
        this->info_message_is_set = true;
    }
    if (!this->invert_visibility_is_set && underlay.invert_visibility_is_set) {
        this->invert_visibility = underlay.invert_visibility;
        this->invert_visibility_is_set = true;
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
    if (!this->maximum_size_on_screen_is_set && underlay.maximum_size_on_screen_is_set) {
        this->maximum_size_on_screen = underlay.maximum_size_on_screen;
        this->maximum_size_on_screen_is_set = true;
    }
    if (!this->minimum_size_on_screen_is_set && underlay.minimum_size_on_screen_is_set) {
        this->minimum_size_on_screen = underlay.minimum_size_on_screen;
        this->minimum_size_on_screen_is_set = true;
    }
    if (!this->mount_filter_is_set && underlay.mount_filter_is_set) {
        this->mount_filter = underlay.mount_filter;
        this->mount_filter_is_set = true;
    }
    if (!this->position_is_set && underlay.position_is_set) {
        this->position = underlay.position;
        this->position_is_set = true;
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
    if (!this->reset_behavior_is_set && underlay.reset_behavior_is_set) {
        this->reset_behavior = underlay.reset_behavior;
        this->reset_behavior_is_set = true;
    }
    if (!this->reset_length_is_set && underlay.reset_length_is_set) {
        this->reset_length = underlay.reset_length;
        this->reset_length_is_set = true;
    }
    if (!this->scale_on_map_with_zoom_is_set && underlay.scale_on_map_with_zoom_is_set) {
        this->scale_on_map_with_zoom = underlay.scale_on_map_with_zoom;
        this->scale_on_map_with_zoom_is_set = true;
    }
    if (!this->schedule_is_set && underlay.schedule_is_set) {
        this->schedule = underlay.schedule;
        this->schedule_is_set = true;
    }
    if (!this->schedule_duration_is_set && underlay.schedule_duration_is_set) {
        this->schedule_duration = underlay.schedule_duration;
        this->schedule_duration_is_set = true;
    }
    if (!this->show_category_is_set && underlay.show_category_is_set) {
        this->show_category = underlay.show_category;
        this->show_category_is_set = true;
    }
    if (!this->specialization_filter_is_set && underlay.specialization_filter_is_set) {
        this->specialization_filter = underlay.specialization_filter;
        this->specialization_filter_is_set = true;
    }
    if (!this->species_filter_is_set && underlay.species_filter_is_set) {
        this->species_filter = underlay.species_filter;
        this->species_filter_is_set = true;
    }
    if (!this->toggle_category_is_set && underlay.toggle_category_is_set) {
        this->toggle_category = underlay.toggle_category;
        this->toggle_category_is_set = true;
    }
    if (!this->tooltip_description_is_set && underlay.tooltip_description_is_set) {
        this->tooltip_description = underlay.tooltip_description;
        this->tooltip_description_is_set = true;
    }
    if (!this->tooltip_name_is_set && underlay.tooltip_name_is_set) {
        this->tooltip_name = underlay.tooltip_name;
        this->tooltip_name_is_set = true;
    }
    if (!this->trigger_range_is_set && underlay.trigger_range_is_set) {
        this->trigger_range = underlay.trigger_range;
        this->trigger_range_is_set = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
// apply_overlay
//
// Transforms this Icon as if the overlay argument were overlayed on
// top of this class.
////////////////////////////////////////////////////////////////////////////////
void Icon::apply_overlay(const Icon& overlay) {
    if (overlay.achievement_bitmask_is_set) {
        this->achievement_bitmask = overlay.achievement_bitmask;
        this->achievement_bitmask_is_set = true;
    }
    if (overlay.achievement_id_is_set) {
        this->achievement_id = overlay.achievement_id;
        this->achievement_id_is_set = true;
    }
    if (overlay.auto_trigger_is_set) {
        this->auto_trigger = overlay.auto_trigger;
        this->auto_trigger_is_set = true;
    }
    if (overlay.bounce_delay_is_set) {
        this->bounce_delay = overlay.bounce_delay;
        this->bounce_delay_is_set = true;
    }
    if (overlay.bounce_duration_is_set) {
        this->bounce_duration = overlay.bounce_duration;
        this->bounce_duration_is_set = true;
    }
    if (overlay.bounce_height_is_set) {
        this->bounce_height = overlay.bounce_height;
        this->bounce_height_is_set = true;
    }
    if (overlay.category_is_set) {
        this->category = overlay.category;
        this->category_is_set = true;
    }
    if (overlay.color_is_set) {
        this->color = overlay.color;
        this->color_is_set = true;
    }
    if (overlay.copy_clipboard_is_set) {
        this->copy_clipboard = overlay.copy_clipboard;
        this->copy_clipboard_is_set = true;
    }
    if (overlay.copy_message_is_set) {
        this->copy_message = overlay.copy_message;
        this->copy_message_is_set = true;
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
    if (overlay.euler_rotation_is_set) {
        this->euler_rotation = overlay.euler_rotation;
        this->euler_rotation_is_set = true;
    }
    if (overlay.festival_filter_is_set) {
        this->festival_filter = overlay.festival_filter;
        this->festival_filter_is_set = true;
    }
    if (overlay.guid_is_set) {
        this->guid = overlay.guid;
        this->guid_is_set = true;
    }
    if (overlay.has_countdown_is_set) {
        this->has_countdown = overlay.has_countdown;
        this->has_countdown_is_set = true;
    }
    if (overlay.height_offset_is_set) {
        this->height_offset = overlay.height_offset;
        this->height_offset_is_set = true;
    }
    if (overlay.hide_category_is_set) {
        this->hide_category = overlay.hide_category;
        this->hide_category_is_set = true;
    }
    if (overlay.icon_is_set) {
        this->icon = overlay.icon;
        this->icon_is_set = true;
    }
    if (overlay.icon_size_is_set) {
        this->icon_size = overlay.icon_size;
        this->icon_size_is_set = true;
    }
    if (overlay.info_message_is_set) {
        this->info_message = overlay.info_message;
        this->info_message_is_set = true;
    }
    if (overlay.invert_visibility_is_set) {
        this->invert_visibility = overlay.invert_visibility;
        this->invert_visibility_is_set = true;
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
    if (overlay.maximum_size_on_screen_is_set) {
        this->maximum_size_on_screen = overlay.maximum_size_on_screen;
        this->maximum_size_on_screen_is_set = true;
    }
    if (overlay.minimum_size_on_screen_is_set) {
        this->minimum_size_on_screen = overlay.minimum_size_on_screen;
        this->minimum_size_on_screen_is_set = true;
    }
    if (overlay.mount_filter_is_set) {
        this->mount_filter = overlay.mount_filter;
        this->mount_filter_is_set = true;
    }
    if (overlay.position_is_set) {
        this->position = overlay.position;
        this->position_is_set = true;
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
    if (overlay.reset_behavior_is_set) {
        this->reset_behavior = overlay.reset_behavior;
        this->reset_behavior_is_set = true;
    }
    if (overlay.reset_length_is_set) {
        this->reset_length = overlay.reset_length;
        this->reset_length_is_set = true;
    }
    if (overlay.scale_on_map_with_zoom_is_set) {
        this->scale_on_map_with_zoom = overlay.scale_on_map_with_zoom;
        this->scale_on_map_with_zoom_is_set = true;
    }
    if (overlay.schedule_is_set) {
        this->schedule = overlay.schedule;
        this->schedule_is_set = true;
    }
    if (overlay.schedule_duration_is_set) {
        this->schedule_duration = overlay.schedule_duration;
        this->schedule_duration_is_set = true;
    }
    if (overlay.show_category_is_set) {
        this->show_category = overlay.show_category;
        this->show_category_is_set = true;
    }
    if (overlay.specialization_filter_is_set) {
        this->specialization_filter = overlay.specialization_filter;
        this->specialization_filter_is_set = true;
    }
    if (overlay.species_filter_is_set) {
        this->species_filter = overlay.species_filter;
        this->species_filter_is_set = true;
    }
    if (overlay.toggle_category_is_set) {
        this->toggle_category = overlay.toggle_category;
        this->toggle_category_is_set = true;
    }
    if (overlay.tooltip_description_is_set) {
        this->tooltip_description = overlay.tooltip_description;
        this->tooltip_description_is_set = true;
    }
    if (overlay.tooltip_name_is_set) {
        this->tooltip_name = overlay.tooltip_name;
        this->tooltip_name_is_set = true;
    }
    if (overlay.trigger_range_is_set) {
        this->trigger_range = overlay.trigger_range;
        this->trigger_range_is_set = true;
    }
}
