#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"
#include <string.h>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string_view>
#include "rapidxml-1.13/rapidxml_print.hpp"

#include "attribute/bool.hpp"
#include "attribute/color.hpp"
#include "attribute/cull_chirality.hpp"
#include "attribute/festival_filter.hpp"
#include "attribute/float.hpp"
#include "attribute/image.hpp"
#include "attribute/int.hpp"
#include "attribute/map_type_filter.hpp"
#include "attribute/marker_category.hpp"
#include "attribute/mount_filter.hpp"
#include "attribute/profession_filter.hpp"
#include "attribute/specialization_filter.hpp"
#include "attribute/species_filter.hpp"
#include "attribute/string.hpp"
#include "attribute/trail_data.hpp"
#include "attribute/trail_data_map_id.hpp"
#include "attribute/unique_id.hpp"
using namespace std;

class Trail: public Parseable {
    public:
        int achievement_bitmask;
        int achievement_id;
        float alpha;
        float animation_speed;
        bool can_fade;
        MarkerCategory category;
        Color color;
        CullChirality cull_chirality;
        float distance_fade_end;
        float distance_fade_start;
        FestivalFilter festival_filter;
        UniqueId guid;
        bool is_wall;
        int map_display_size;
        int map_id;
        MapTypeFilter map_type_filter;
        MountFilter mount_filter;
        ProfessionFilter profession_filter;
        bool render_ingame;
        bool render_on_map;
        bool render_on_minimap;
        string schedule;
        float schedule_duration;
        SpecializationFilter specialization_filter;
        SpeciesFilter species_filter;
        Image texture;
        TrailData trail_data;
        TrailDataMapId trail_data_map_id;
        float trail_scale;
        virtual string classname();
        bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<XMLError*> *errors);
        bool validate_attributes_of_type_marker_category();
};