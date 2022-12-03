#include "icon_gen.hpp"
#include "waypoint.pb.h"

#include <iosfwd>
#include <string>

#include "attribute/bool.hpp"
#include "attribute/float.hpp"
#include "attribute/int.hpp"
#include "attribute/string.hpp"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "string_helper.hpp"

using namespace std;

string Icon::classname() {
    return "POI";
}

bool Icon::init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*>* errors) {
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
    else if (attributename == "alpha") {
        this->alpha = parse_float(attribute, errors);
        this->alpha_is_set = true;
    }
    else if (attributename == "autotrigger") {
        this->auto_trigger = parse_bool(attribute, errors);
        this->auto_trigger_is_set = true;
    }
    else if (attributename == "bouncedelay") {
        this->bounce_delay = parse_float(attribute, errors);
        this->bounce_delay_is_set = true;
    }
    else if (attributename == "bounceduration") {
        this->bounce_duration = parse_float(attribute, errors);
        this->bounce_duration_is_set = true;
    }
    else if (attributename == "bounceheight") {
        this->bounce_height = parse_float(attribute, errors);
        this->bounce_height_is_set = true;
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
    else if (attributename == "copy") {
        this->copy_clipboard = parse_string(attribute, errors);
        this->copy_clipboard_is_set = true;
    }
    else if (attributename == "copyclipboard") {
        this->copy_clipboard = parse_string(attribute, errors);
        this->copy_clipboard_is_set = true;
    }
    else if (attributename == "copymessage") {
        this->copy_message = parse_string(attribute, errors);
        this->copy_message_is_set = true;
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
    else if (attributename == "rotate") {
        this->euler_rotation = parse_euler_rotation(attribute, errors);
        this->euler_rotation_is_set = true;
    }
    else if (attributename == "festival") {
        this->festival_filter = parse_festival_filter(attribute, errors);
        this->festival_filter_is_set = true;
    }
    else if (attributename == "guid") {
        this->guid = parse_unique_id(attribute, errors);
        this->guid_is_set = true;
    }
    else if (attributename == "hascountdown") {
        this->has_countdown = parse_bool(attribute, errors);
        this->has_countdown_is_set = true;
    }
    else if (attributename == "heightoffset") {
        this->heightoffset = parse_float(attribute, errors);
        this->heightoffset_is_set = true;
    }
    else if (attributename == "hide") {
        this->hide_category = parse_marker_category(attribute, errors);
        this->hide_category_is_set = true;
    }
    else if (attributename == "iconfile") {
        this->icon = parse_image(attribute, errors);
        this->icon_is_set = true;
    }
    else if (attributename == "iconsize") {
        this->icon_size = parse_float(attribute, errors);
        this->icon_size_is_set = true;
    }
    else if (attributename == "info") {
        this->info_message = parse_string(attribute, errors);
        this->info_message_is_set = true;
    }
    else if (attributename == "invertbehavior") {
        this->invert_visibility = parse_bool(attribute, errors);
        this->invert_visibility_is_set = true;
    }
    else if (attributename == "mapdisplaysize") {
        this->map_display_size = parse_int(attribute, errors);
        this->map_display_size_is_set = true;
    }
    else if (attributename == "mapid") {
        this->map_id = parse_int(attribute, errors);
        this->map_id_is_set = true;
    }
    else if (attributename == "mapid") {
        this->map_type_filter = parse_map_type_filter(attribute, errors);
        this->map_type_filter_is_set = true;
    }
    else if (attributename == "maxsize") {
        this->maximum_size_on_screen = parse_int(attribute, errors);
        this->maximum_size_on_screen_is_set = true;
    }
    else if (attributename == "minsize") {
        this->minimum_size_on_screen = parse_int(attribute, errors);
        this->minimum_size_on_screen_is_set = true;
    }
    else if (attributename == "mount") {
        this->mount_filter = parse_mount_filter(attribute, errors);
        this->mount_filter_is_set = true;
    }
    else if (attributename == "position") {
        this->position = parse_position(attribute, errors);
        this->position_is_set = true;
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
    else if (attributename == "behavior") {
        this->reset_behavior = parse_reset_behavior(attribute, errors);
        this->reset_behavior_is_set = true;
    }
    else if (attributename == "resetlength") {
        this->reset_length = parse_float(attribute, errors);
        this->reset_length_is_set = true;
    }
    else if (attributename == "scaleonmapwithzoom") {
        this->scale_on_map_with_zoom = parse_bool(attribute, errors);
        this->scale_on_map_with_zoom_is_set = true;
    }
    else if (attributename == "schedule") {
        this->schedule = parse_string(attribute, errors);
        this->schedule_is_set = true;
    }
    else if (attributename == "scheduleduration") {
        this->schedule_duration = parse_float(attribute, errors);
        this->schedule_duration_is_set = true;
    }
    else if (attributename == "show") {
        this->show_category = parse_marker_category(attribute, errors);
        this->show_category_is_set = true;
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
    else if (attributename == "toggle") {
        this->toggle_category = parse_marker_category(attribute, errors);
        this->toggle_category_is_set = true;
    }
    else if (attributename == "togglecategory") {
        this->toggle_category = parse_marker_category(attribute, errors);
        this->toggle_category_is_set = true;
    }
    else if (attributename == "tipdescription") {
        this->tooltip_description = parse_string(attribute, errors);
        this->tooltip_description_is_set = true;
    }
    else if (attributename == "tipname") {
        this->tooltip_name = parse_string(attribute, errors);
        this->tooltip_name_is_set = true;
    }
    else if (attributename == "triggerrange") {
        this->trigger_range = parse_float(attribute, errors);
        this->trigger_range_is_set = true;
    }
    else if (attributename == "inforange") {
        this->trigger_range = parse_float(attribute, errors);
        this->trigger_range_is_set = true;
    }
    else if (attributename == "xpos") {
        this->x_position = parse_float(attribute, errors);
        this->x_position_is_set = true;
    }
    else if (attributename == "positionx") {
        this->x_position = parse_float(attribute, errors);
        this->x_position_is_set = true;
    }
    else if (attributename == "rotatex") {
        this->x_rotation = parse_float(attribute, errors);
        this->x_rotation_is_set = true;
    }
    else if (attributename == "ypos") {
        this->y_position = parse_float(attribute, errors);
        this->y_position_is_set = true;
    }
    else if (attributename == "positiony") {
        this->y_position = parse_float(attribute, errors);
        this->y_position_is_set = true;
    }
    else if (attributename == "rotatey") {
        this->y_rotation = parse_float(attribute, errors);
        this->y_rotation_is_set = true;
    }
    else if (attributename == "zpos") {
        this->z_position = parse_float(attribute, errors);
        this->z_position_is_set = true;
    }
    else if (attributename == "positionz") {
        this->z_position = parse_float(attribute, errors);
        this->z_position_is_set = true;
    }
    else if (attributename == "rotatez") {
        this->z_rotation = parse_float(attribute, errors);
        this->z_rotation_is_set = true;
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

vector<string> Icon::as_xml() const {
    vector<string> xml_node_contents;
    xml_node_contents.push_back("<POI ");
    if (this->achievement_bitmask_is_set) {
        xml_node_contents.push_back(" AchievementBit=\"" + stringify_int(this->achievement_bitmask) + "\"");
    }
    if (this->achievement_id_is_set) {
        xml_node_contents.push_back(" AchievementId=\"" + stringify_int(this->achievement_id) + "\"");
    }
    if (this->alpha_is_set) {
        xml_node_contents.push_back(" Alpha=\"" + stringify_float(this->alpha) + "\"");
    }
    if (this->auto_trigger_is_set) {
        xml_node_contents.push_back(" AutoTrigger=\"" + stringify_bool(this->auto_trigger) + "\"");
    }
    if (this->bounce_delay_is_set) {
        xml_node_contents.push_back(" BounceDelay=\"" + stringify_float(this->bounce_delay) + "\"");
    }
    if (this->bounce_duration_is_set) {
        xml_node_contents.push_back(" BounceDuration=\"" + stringify_float(this->bounce_duration) + "\"");
    }
    if (this->bounce_height_is_set) {
        xml_node_contents.push_back(" BounceHeight=\"" + stringify_float(this->bounce_height) + "\"");
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
    if (this->copy_clipboard_is_set) {
        xml_node_contents.push_back(" Copy=\"" + stringify_string(this->copy_clipboard) + "\"");
    }
    if (this->copy_message_is_set) {
        xml_node_contents.push_back(" CopyMessage=\"" + stringify_string(this->copy_message) + "\"");
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
    if (this->euler_rotation_is_set) {
        xml_node_contents.push_back(" Rotate=\"" + stringify_euler_rotation(this->euler_rotation) + "\"");
    }
    if (this->festival_filter_is_set) {
        xml_node_contents.push_back(" Festival=\"" + stringify_festival_filter(this->festival_filter) + "\"");
    }
    if (this->guid_is_set) {
        xml_node_contents.push_back(" GUID=\"" + stringify_unique_id(this->guid) + "\"");
    }
    if (this->has_countdown_is_set) {
        xml_node_contents.push_back(" HasCountdown=\"" + stringify_bool(this->has_countdown) + "\"");
    }
    if (this->heightoffset_is_set) {
        xml_node_contents.push_back(" HeightOffset=\"" + stringify_float(this->heightoffset) + "\"");
    }
    if (this->hide_category_is_set) {
        xml_node_contents.push_back(" Hide=\"" + stringify_marker_category(this->hide_category) + "\"");
    }
    if (this->icon_is_set) {
        xml_node_contents.push_back(" IconFile=\"" + stringify_image(this->icon) + "\"");
    }
    if (this->icon_size_is_set) {
        xml_node_contents.push_back(" IconSize=\"" + stringify_float(this->icon_size) + "\"");
    }
    if (this->info_message_is_set) {
        xml_node_contents.push_back(" Info=\"" + stringify_string(this->info_message) + "\"");
    }
    if (this->invert_visibility_is_set) {
        xml_node_contents.push_back(" InvertBehavior=\"" + stringify_bool(this->invert_visibility) + "\"");
    }
    if (this->map_display_size_is_set) {
        xml_node_contents.push_back(" MapDisplaySize=\"" + stringify_int(this->map_display_size) + "\"");
    }
    if (this->map_id_is_set) {
        xml_node_contents.push_back(" MapID=\"" + stringify_int(this->map_id) + "\"");
    }
    if (this->map_type_filter_is_set) {
        xml_node_contents.push_back(" MapID=\"" + stringify_map_type_filter(this->map_type_filter) + "\"");
    }
    if (this->maximum_size_on_screen_is_set) {
        xml_node_contents.push_back(" MaxSize=\"" + stringify_int(this->maximum_size_on_screen) + "\"");
    }
    if (this->minimum_size_on_screen_is_set) {
        xml_node_contents.push_back(" MinSize=\"" + stringify_int(this->minimum_size_on_screen) + "\"");
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
    if (this->reset_behavior_is_set) {
        xml_node_contents.push_back(" Behavior=\"" + stringify_reset_behavior(this->reset_behavior) + "\"");
    }
    if (this->reset_length_is_set) {
        xml_node_contents.push_back(" ResetLength=\"" + stringify_float(this->reset_length) + "\"");
    }
    if (this->scale_on_map_with_zoom_is_set) {
        xml_node_contents.push_back(" ScaleOnMapWithZoom=\"" + stringify_bool(this->scale_on_map_with_zoom) + "\"");
    }
    if (this->schedule_is_set) {
        xml_node_contents.push_back(" Schedule=\"" + stringify_string(this->schedule) + "\"");
    }
    if (this->schedule_duration_is_set) {
        xml_node_contents.push_back(" ScheduleDuration=\"" + stringify_float(this->schedule_duration) + "\"");
    }
    if (this->show_category_is_set) {
        xml_node_contents.push_back(" Show=\"" + stringify_marker_category(this->show_category) + "\"");
    }
    if (this->specialization_filter_is_set) {
        xml_node_contents.push_back(" Specialization=\"" + stringify_specialization_filter(this->specialization_filter) + "\"");
    }
    if (this->species_filter_is_set) {
        xml_node_contents.push_back(" Race=\"" + stringify_species_filter(this->species_filter) + "\"");
    }
    if (this->toggle_category_is_set) {
        xml_node_contents.push_back(" Toggle=\"" + stringify_marker_category(this->toggle_category) + "\"");
    }
    if (this->tooltip_description_is_set) {
        xml_node_contents.push_back(" TipDescription=\"" + stringify_string(this->tooltip_description) + "\"");
    }
    if (this->tooltip_name_is_set) {
        xml_node_contents.push_back(" TipName=\"" + stringify_string(this->tooltip_name) + "\"");
    }
    if (this->trigger_range_is_set) {
        xml_node_contents.push_back(" TriggerRange=\"" + stringify_float(this->trigger_range) + "\"");
    }
    if (this->x_position_is_set) {
        xml_node_contents.push_back(" XPos=\"" + to_string(this->x_position) + "\"");
    }
    if (this->y_position_is_set) {
        xml_node_contents.push_back(" YPos=\"" + to_string(this->y_position) + "\"");
    }
    if (this->z_position_is_set) {
        xml_node_contents.push_back(" ZPos=\"" + to_string(this->z_position) + "\"");
    }
    xml_node_contents.push_back("/>");
    return xml_node_contents;
}

waypoint::Icon Icon::as_protobuf(waypoint::Icon proto_icon) const {
    waypoint::Trigger* trigger = new waypoint::Trigger();
    bool set_trigger = false;
 
    if (this->achievement_bitmask_is_set) {
        proto_icon.set_achievement_bit(to_proto_int(this->achievement_bitmask));
    }
    
    if (this->achievement_id_is_set) {
        proto_icon.set_achievement_id(to_proto_int(this->achievement_id));
    }
    
    if (this->alpha_is_set) {
        proto_icon.set_alpha(to_proto_float(this->alpha));
    }
    
    if (this->auto_trigger_is_set) {
        trigger->set_auto_trigger(to_proto_bool(this->auto_trigger));
        set_trigger = true;
    }
    
    if (this->bounce_delay_is_set) {
        trigger->set_bounce_delay(to_proto_float(this->bounce_delay));
        set_trigger = true;
    }
    
    if (this->bounce_duration_is_set) {
        trigger->set_bounce_duration(to_proto_float(this->bounce_duration));
        set_trigger = true;
    }
    
    if (this->bounce_height_is_set) {
        trigger->set_bounce_height(to_proto_float(this->bounce_height));
        set_trigger = true;
    }
    
    if (this->can_fade_is_set) {
        proto_icon.set_can_fade(to_proto_bool(this->can_fade));
    }
    
    if (this->category_is_set) {
        proto_icon.set_allocated_category(to_proto_marker_category(this->category));
    }
    
    if (this->color_is_set) {
        proto_icon.set_allocated_color(to_proto_color(this->color));
    }
    
    if (this->copy_clipboard_is_set) {
        trigger->set_action_copy_clipboard(to_proto_string(this->copy_clipboard));
        set_trigger = true;
    }
    
    if (this->copy_message_is_set) {
        trigger->set_action_copy_message(to_proto_string(this->copy_message));
        set_trigger = true;
    }
    
    if (this->cull_chirality_is_set) {
        proto_icon.set_cull_chirality(to_proto_cull_chirality(this->cull_chirality)); 
    }
    
    if (this->distance_fade_end_is_set) {
        proto_icon.set_distance_fade_end(to_proto_float(this->distance_fade_end));
    }
    
    if (this->distance_fade_start_is_set) {
        proto_icon.set_distance_fade_start(to_proto_float(this->distance_fade_start));
    }
    
    if (this->euler_rotation_is_set) {
        proto_icon.set_allocated_euler_rotation(to_proto_euler_rotation(this->euler_rotation));
    }
    
    if (this->festival_filter_is_set) {
        proto_icon.set_allocated_festival_filter(to_proto_festival_filter(this->festival_filter));
    }
    
    if (this->guid_is_set) {
        proto_icon.set_allocated_guid(to_proto_unique_id(this->guid));
    }
    
    if (this->has_countdown_is_set) {
        trigger->set_has_countdown(to_proto_bool(this->has_countdown));
        set_trigger = true;
    }
    
    if (this->heightoffset_is_set) {
        proto_icon.set_height_offset(to_proto_float(this->heightoffset));
    }
    
    if (this->hide_category_is_set) {
        trigger->set_allocated_action_hide_category(to_proto_marker_category(this->hide_category));
        set_trigger = true;
    }
    
    if (this->icon_is_set) {
        proto_icon.set_allocated_texture(to_proto_image(this->icon));
    }
    
    if (this->icon_size_is_set) {
        proto_icon.set___tentative__scale(to_proto_float(this->icon_size));
    }
    
    if (this->info_message_is_set) {
        trigger->set_action_info_message(to_proto_string(this->info_message));
        set_trigger = true;
    }
    
    if (this->invert_visibility_is_set) {
        trigger->set_invert_display(to_proto_bool(this->invert_visibility));
        set_trigger = true;
    }
    
    if (this->map_display_size_is_set) {
        proto_icon.set_map_display_size(to_proto_int(this->map_display_size));
    }
    
    if (this->map_id_is_set) {
        proto_icon.set_map_id(to_proto_int(this->map_id));
    }
    
    if (this->map_type_filter_is_set) {
        proto_icon.set_allocated_map_type_filter(to_proto_map_type_filter(this->map_type_filter));
    }
    
    if (this->maximum_size_on_screen_is_set) {
        proto_icon.set_maximum_size_on_screen(to_proto_int(this->maximum_size_on_screen));
    }
    
    if (this->minimum_size_on_screen_is_set) {
        proto_icon.set_minimum_size_on_screen(to_proto_int(this->minimum_size_on_screen));
    }
    
    if (this->mount_filter_is_set) {
        proto_icon.set_allocated_mount_filter(to_proto_mount_filter(this->mount_filter));
    }
    
    if (this->position_is_set) {
        proto_icon.set_allocated_position(to_proto_position(this->position));
    }
    
    if (this->profession_filter_is_set) {
        proto_icon.set_allocated_profession_filter(to_proto_profession_filter(this->profession_filter));
    }
    
    if (this->render_ingame_is_set) {
        proto_icon.set___tentative__render_ingame(to_proto_bool(this->render_ingame));
    }
    
    if (this->render_on_map_is_set) {
        proto_icon.set___tentative__render_on_map(to_proto_bool(this->render_on_map));
    }
    
    if (this->render_on_minimap_is_set) {
        proto_icon.set___tentative__render_on_minimap(to_proto_bool(this->render_on_minimap));
    }
    
    if (this->reset_behavior_is_set) {
        trigger->set_reset_behavior(to_proto_reset_behavior(this->reset_behavior));
        set_trigger = true;
    }
    
    if (this->reset_length_is_set) {
        trigger->set_reset_length(to_proto_float(this->reset_length));
        set_trigger = true;
    }
    
    if (this->scale_on_map_with_zoom_is_set) {
        proto_icon.set_scale_on_map_with_zoom(to_proto_bool(this->scale_on_map_with_zoom));
    }
    
    if (this->schedule_is_set) {
        proto_icon.set_bhdraft__schedule(to_proto_string(this->schedule));
    }
    
    if (this->schedule_duration_is_set) {
        proto_icon.set_bhdraft__schedule_duration(to_proto_float(this->schedule_duration));
    }
    
    if (this->show_category_is_set) {
        trigger->set_allocated_action_show_category(to_proto_marker_category(this->show_category));
        set_trigger = true;
    }
    
    if (this->specialization_filter_is_set) {
        proto_icon.set_allocated_specialization_filter(to_proto_specialization_filter(this->specialization_filter));
    }
    
    if (this->species_filter_is_set) {
        proto_icon.set_allocated_species_filter(to_proto_species_filter(this->species_filter));
    }
    
    if (this->toggle_category_is_set) {
        trigger->set_allocated_action_toggle_category(to_proto_marker_category(this->toggle_category));
        set_trigger = true;
    }
    
    if (this->tooltip_description_is_set) {
        proto_icon.set_tip_description(to_proto_string(this->tooltip_description));
    }
    
    if (this->tooltip_name_is_set) {
        proto_icon.set_tip_name(to_proto_string(this->tooltip_name));
    }
    
    if (this->trigger_range_is_set) {
        trigger->set_range(to_proto_float(this->trigger_range));
        set_trigger = true;
    }
    
    
    
    
    
    
    
    if (set_trigger){
            proto_icon.set_allocated_trigger(trigger);
    }
 
    return proto_icon;
}
    
// Icon::from_protobuf()

