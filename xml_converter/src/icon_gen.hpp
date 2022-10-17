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
        int achievement_id;
        float alpha;
        bool auto_trigger;
        float bounce_delay;
        float bounce_duration;
        float bounce_height;
        bool can_fade;
        MarkerCategory category;
        Color color;
        string copy_clipboard;
        string copy_message;
        CullChirality cull_chirality;
        float distance_fade_end;
        float distance_fade_start;
        EulerRotation euler_rotation;
        FestivalFilter festival_filter;
        UniqueId guid;
        bool has_countdown;
        float heightoffset;
        MarkerCategory hide_category;
        Image icon;
        float icon_size;
        string info_message;
        bool invert_visibility;
        int map_display_size;
        int map_id;
        MapTypeFilter map_type_filter;
        int maximum_size_on_screen;
        int minimum_size_on_screen;
        MountFilter mount_filter;
        Position position;
        ProfessionFilter profession_filter;
        bool render_ingame;
        bool render_on_map;
        bool render_on_minimap;
        ResetBehavior reset_behavior;
        float reset_length;
        bool scale_on_map_with_zoom;
        string schedule;
        float schedule_duration;
        MarkerCategory show_category;
        SpecializationFilter specialization_filter;
        SpeciesFilter species_filter;
        MarkerCategory toggle_category;
        string tooltip_description;
        string tooltip_name;
        float trigger_range;
        float x_position;
        float x_rotation;
        float y_position;
        float y_rotation;
        float z_position;
        float z_rotation;
        bool achievement_bitmask_is_set = false;
        bool achievement_id_is_set = false;
        bool alpha_is_set = false;
        bool auto_trigger_is_set = false;
        bool bounce_delay_is_set = false;
        bool bounce_duration_is_set = false;
        bool bounce_height_is_set = false;
        bool can_fade_is_set = false;
        bool category_is_set = false;
        bool color_is_set = false;
        bool copy_clipboard_is_set = false;
        bool copy_message_is_set = false;
        bool cull_chirality_is_set = false;
        bool distance_fade_end_is_set = false;
        bool distance_fade_start_is_set = false;
        bool euler_rotation_is_set = false;
        bool festival_filter_is_set = false;
        bool guid_is_set = false;
        bool has_countdown_is_set = false;
        bool heightoffset_is_set = false;
        bool hide_category_is_set = false;
        bool icon_is_set = false;
        bool icon_size_is_set = false;
        bool info_message_is_set = false;
        bool invert_visibility_is_set = false;
        bool map_display_size_is_set = false;
        bool map_id_is_set = false;
        bool map_type_filter_is_set = false;
        bool maximum_size_on_screen_is_set = false;
        bool minimum_size_on_screen_is_set = false;
        bool mount_filter_is_set = false;
        bool position_is_set = false;
        bool profession_filter_is_set = false;
        bool render_ingame_is_set = false;
        bool render_on_map_is_set = false;
        bool render_on_minimap_is_set = false;
        bool reset_behavior_is_set = false;
        bool reset_length_is_set = false;
        bool scale_on_map_with_zoom_is_set = false;
        bool schedule_is_set = false;
        bool schedule_duration_is_set = false;
        bool show_category_is_set = false;
        bool specialization_filter_is_set = false;
        bool species_filter_is_set = false;
        bool toggle_category_is_set = false;
        bool tooltip_description_is_set = false;
        bool tooltip_name_is_set = false;
        bool trigger_range_is_set = false;
        bool x_position_is_set = false;
        bool x_rotation_is_set = false;
        bool y_position_is_set = false;
        bool y_rotation_is_set = false;
        bool z_position_is_set = false;
        bool z_rotation_is_set = false;
        virtual vector<string> as_xml() const;
        virtual string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);
        bool validate_attributes_of_type_marker_category();
};