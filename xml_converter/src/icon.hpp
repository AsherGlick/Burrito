#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"

#include "attribute/bool.hpp"
#include "attribute/color.hpp"
#include "attribute/cull_chirality.hpp"
#include "attribute/euler_rotation.hpp"
#include "attribute/festival_filter.hpp"
#include "attribute/float.hpp"
#include "attribute/image.hpp"
#include "attribute/int.hpp"
#include "attribute/map_type_filter.hpp"
#include "attribute/marker_category.hpp"
#include "attribute/mount_filter.hpp"
#include "attribute/position.hpp"
#include "attribute/profession_filter.hpp"
#include "attribute/reset_behavior.hpp"
#include "attribute/specialization_filter.hpp"
#include "attribute/species_filter.hpp"
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
        virtual string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);
        bool validate_attributes_of_type_marker_category();
};