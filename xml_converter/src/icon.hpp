#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"

#include "bool.hpp"
#include "category.hpp"
#include "color.hpp"
#include "cull_chirality.hpp"
#include "euler_rotation.hpp"
#include "festival_filter.hpp"
#include "float.hpp"
#include "image.hpp"
#include "int.hpp"
#include "map_type_filter.hpp"
#include "mount_filter.hpp"
#include "position.hpp"
#include "profession_filter.hpp"
#include "reset_behavior.hpp"
#include "specialization_filter.hpp"
#include "species_filter.hpp"
#include "string.hpp"
#include "uniqueid.hpp"

using namespace std;

class Icon: public Parseable {
    private:
        int achievement_bitmask;
        int achievement_id;
        float alpha;
        bool auto-trigger;
        float bounce_delay;
        float bounce_duration;
        float bounce_height;
        bool can_fade;
        Category category;
        Color color;
        string copy_clipboard;
        string copy_message;
        CullChirality cull_chirality;
        float distance_fade_end;
        float distance_fade_start;
        EulerRotation euler_rotation;
        FestivalFilter festival_filter;
        UniqueID guid;
        bool has_countdown;
        float heightoffset;
        Category hide_category;
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
        Category show_category;
        SpecializationFilter specialization_filter;
        SpeciesFilter species_filter;
        Category toggle_category;
        string tooltip_description;
        string tooltip_name;
        float trigger_range;

        virtual string classname();
};