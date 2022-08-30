#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"

#include "attribute/bool.hpp"
#include "attribute/color.hpp"
#include "attribute/cull_chirality_gen.hpp"
#include "attribute/euler_rotation_gen.hpp"
#include "attribute/festival_filter_gen.hpp"
#include "attribute/float.hpp"
#include "attribute/image.hpp"
#include "attribute/int.hpp"
#include "attribute/map_type_filter_gen.hpp"
#include "attribute/marker_category.hpp"
#include "attribute/mount_filter_gen.hpp"
#include "attribute/position_gen.hpp"
#include "attribute/profession_filter_gen.hpp"
#include "attribute/reset_behavior_gen.hpp"
#include "attribute/specialization_filter_gen.hpp"
#include "attribute/species_filter_gen.hpp"
#include "attribute/string.hpp"
#include "attribute/unique_id.hpp"
using namespace std;

class Icon: public Parseable {
    public:
        int achievement_bitmask;
        bool achievement_bitmask_is_set = false;
        int achievement_id;
        bool achievement_id_is_set = false;
        float alpha;
        bool alpha_is_set = false;
        bool auto_trigger;
        bool auto_trigger_is_set = false;
        float bounce_delay;
        bool bounce_delay_is_set = false;
        float bounce_duration;
        bool bounce_duration_is_set = false;
        float bounce_height;
        bool bounce_height_is_set = false;
        bool can_fade;
        bool can_fade_is_set = false;
        MarkerCategory category;
        bool category_is_set = false;
        Color color;
        bool color_is_set = false;
        string copy_clipboard;
        bool copy_clipboard_is_set = false;
        string copy_message;
        bool copy_message_is_set = false;
        CullChirality cull_chirality;
        bool cull_chirality_is_set = false;
        float distance_fade_end;
        bool distance_fade_end_is_set = false;
        float distance_fade_start;
        bool distance_fade_start_is_set = false;
        EulerRotation euler_rotation;
        bool euler_rotation_is_set = false;
        FestivalFilter festival_filter;
        bool festival_filter_is_set = false;
        UniqueId guid;
        bool guid_is_set = false;
        bool has_countdown;
        bool has_countdown_is_set = false;
        float heightoffset;
        bool heightoffset_is_set = false;
        MarkerCategory hide_category;
        bool hide_category_is_set = false;
        Image icon;
        bool icon_is_set = false;
        float icon_size;
        bool icon_size_is_set = false;
        string info_message;
        bool info_message_is_set = false;
        bool invert_visibility;
        bool invert_visibility_is_set = false;
        int map_display_size;
        bool map_display_size_is_set = false;
        int map_id;
        bool map_id_is_set = false;
        MapTypeFilter map_type_filter;
        bool map_type_filter_is_set = false;
        int maximum_size_on_screen;
        bool maximum_size_on_screen_is_set = false;
        int minimum_size_on_screen;
        bool minimum_size_on_screen_is_set = false;
        MountFilter mount_filter;
        bool mount_filter_is_set = false;
        Position position;
        bool position_is_set = false;
        ProfessionFilter profession_filter;
        bool profession_filter_is_set = false;
        bool render_ingame;
        bool render_ingame_is_set = false;
        bool render_on_map;
        bool render_on_map_is_set = false;
        bool render_on_minimap;
        bool render_on_minimap_is_set = false;
        ResetBehavior reset_behavior;
        bool reset_behavior_is_set = false;
        float reset_length;
        bool reset_length_is_set = false;
        bool scale_on_map_with_zoom;
        bool scale_on_map_with_zoom_is_set = false;
        string schedule;
        bool schedule_is_set = false;
        float schedule_duration;
        bool schedule_duration_is_set = false;
        MarkerCategory show_category;
        bool show_category_is_set = false;
        SpecializationFilter specialization_filter;
        bool specialization_filter_is_set = false;
        SpeciesFilter species_filter;
        bool species_filter_is_set = false;
        MarkerCategory toggle_category;
        bool toggle_category_is_set = false;
        string tooltip_description;
        bool tooltip_description_is_set = false;
        string tooltip_name;
        bool tooltip_name_is_set = false;
        float trigger_range;
        bool trigger_range_is_set = false;
        float x_position;
        bool x_position_is_set = false;
        float x_rotation;
        bool x_rotation_is_set = false;
        float y_position;
        bool y_position_is_set = false;
        float y_rotation;
        bool y_rotation_is_set = false;
        float z_position;
        bool z_position_is_set = false;
        float z_rotation;
        bool z_rotation_is_set = false;
        virtual string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);
        virtual vector<string> as_xml() const;
        bool validate_attributes_of_type_marker_category();
};