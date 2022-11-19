#pragma once

#include <string>
#include <vector>

#include "attribute/color.hpp"
#include "attribute/cull_chirality_gen.hpp"
#include "attribute/festival_filter_gen.hpp"
#include "attribute/image.hpp"
#include "attribute/map_type_filter_gen.hpp"
#include "attribute/marker_category.hpp"
#include "attribute/mount_filter_gen.hpp"
#include "attribute/profession_filter_gen.hpp"
#include "attribute/specialization_filter_gen.hpp"
#include "attribute/species_filter_gen.hpp"
#include "attribute/trail_data.hpp"
#include "attribute/unique_id.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
class XMLError;

class Trail : public Parseable {
 public:
    int achievement_bitmask;
    bool achievement_bitmask_is_set = false;
    int achievement_id;
    bool achievement_id_is_set = false;
    float alpha;
    bool alpha_is_set = false;
    float animation_speed;
    bool animation_speed_is_set = false;
    bool can_fade;
    bool can_fade_is_set = false;
    MarkerCategory category;
    bool category_is_set = false;
    Color color;
    bool color_is_set = false;
    CullChirality cull_chirality;
    bool cull_chirality_is_set = false;
    float distance_fade_end;
    bool distance_fade_end_is_set = false;
    float distance_fade_start;
    bool distance_fade_start_is_set = false;
    FestivalFilter festival_filter;
    bool festival_filter_is_set = false;
    UniqueId guid;
    bool guid_is_set = false;
    bool is_wall;
    bool is_wall_is_set = false;
    int map_display_size;
    bool map_display_size_is_set = false;
    int map_id;
    bool map_id_is_set = false;
    MapTypeFilter map_type_filter;
    bool map_type_filter_is_set = false;
    MountFilter mount_filter;
    bool mount_filter_is_set = false;
    ProfessionFilter profession_filter;
    bool profession_filter_is_set = false;
    bool render_ingame;
    bool render_ingame_is_set = false;
    bool render_on_map;
    bool render_on_map_is_set = false;
    bool render_on_minimap;
    bool render_on_minimap_is_set = false;
    std::string schedule;
    bool schedule_is_set = false;
    float schedule_duration;
    bool schedule_duration_is_set = false;
    SpecializationFilter specialization_filter;
    bool specialization_filter_is_set = false;
    SpeciesFilter species_filter;
    bool species_filter_is_set = false;
    Image texture;
    bool texture_is_set = false;
    TrailData trail_data;
    bool trail_data_is_set = false;
    float trail_scale;
    bool trail_scale_is_set = false;
    virtual std::string classname();
    bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, std::vector<XMLError*>* errors);
    virtual std::vector<std::string> as_xml() const;
    bool validate_attributes_of_type_marker_category();
};
