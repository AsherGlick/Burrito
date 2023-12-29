#pragma once

#include <functional>
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
#include "state_structs/xml_reader_state.hpp"
#include "waypoint.pb.h"

class XMLError;

class Trail : public Parseable {
 public:
    int achievement_bitmask;
    int achievement_id;
    float animation_speed;
    MarkerCategory category;
    Color color;
    CullChirality cull_chirality;
    bool disable_player_cutout;
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
    std::string schedule;
    float schedule_duration;
    SpecializationFilter specialization_filter;
    SpeciesFilter species_filter;
    Image texture;
    TrailData trail_data;
    float trail_scale;
    bool achievement_bitmask_is_set = false;
    bool achievement_id_is_set = false;
    bool animation_speed_is_set = false;
    bool category_is_set = false;
    bool color_is_set = false;
    bool cull_chirality_is_set = false;
    bool disable_player_cutout_is_set = false;
    bool distance_fade_end_is_set = false;
    bool distance_fade_start_is_set = false;
    bool festival_filter_is_set = false;
    bool guid_is_set = false;
    bool is_wall_is_set = false;
    bool map_display_size_is_set = false;
    bool map_id_is_set = false;
    bool map_type_filter_is_set = false;
    bool mount_filter_is_set = false;
    bool profession_filter_is_set = false;
    bool render_ingame_is_set = false;
    bool render_on_map_is_set = false;
    bool render_on_minimap_is_set = false;
    bool schedule_is_set = false;
    bool schedule_duration_is_set = false;
    bool specialization_filter_is_set = false;
    bool species_filter_is_set = false;
    bool texture_is_set = false;
    bool trail_data_is_set = false;
    bool trail_scale_is_set = false;
    virtual std::vector<std::string> as_xml(XMLWriterState* state) const;
    virtual std::string classname();
    bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, std::vector<XMLError*>* errors, XMLReaderState* state);
    waypoint::Trail as_protobuf(ProtoWriterState* state) const;
    void parse_protobuf(waypoint::Trail proto_trail, ProtoReaderState* state);
    bool validate_attributes_of_type_marker_category();
};
