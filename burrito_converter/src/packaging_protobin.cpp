#include "packaging_protobin.hpp"

#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "category_gen.hpp"
#include "file_helper.hpp"
#include "guildpoint.pb.h"
#include "parseable.hpp"
#include "state_structs/proto_writer_state.hpp"
#include "string_helper.hpp"
#include "string_hierarchy.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SERIALIZE ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
Category* parse_guildpoint_categories(
    string full_category_name,
    ::guildpoint::Category proto_category,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois,
    ProtoReaderState* state
) {
    string category_name = normalize(proto_category.name());
    full_category_name += category_name;
    Category* this_category = &(*marker_categories)[category_name];

    this_category->parse_protobuf(proto_category, state);

    for (int i = 0; i < proto_category.icon_size(); i++) {
        Icon* icon = new Icon();
        icon->parse_protobuf(proto_category.icon(i), state);
        // TODO: The field category in Icon is being deprciated
        // This overwrites any icon.category with its position in the heirarchy
        icon->category.category = full_category_name;
        icon->category_is_set = true;
        parsed_pois->push_back(icon);
    }
    for (int i = 0; i < proto_category.trail_size(); i++) {
        Trail* trail = new Trail();
        trail->parse_protobuf(proto_category.trail(i), state);
        // TODO: The field category in Trail is being deprciated
        // This overwrites any trail.category with its position in the heirarchy
        trail->category.category = full_category_name;
        trail->category_is_set = true;
        parsed_pois->push_back(trail);
    }

    for (int i = 0; i < proto_category.children_size(); i++) {
        parse_guildpoint_categories(full_category_name + ".", proto_category.children(i), &(this_category->children), parsed_pois, state);
    }
    return this_category;
}

////////////////////////////////////////////////////////////////////////////////
// read_protobuf_file
//
// Reads a protobuf file into memory.
////////////////////////////////////////////////////////////////////////////////
map<Attribute::UniqueId::UniqueId, Category*> read_protobuf_file(
    const MarkerPackFile& proto_filepath,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois
) {
    unique_ptr<basic_istream<char>> infile = read_file(proto_filepath);

    guildpoint::Guildpoint proto_message;
    proto_message.ParseFromIstream(&*infile);

    ProtoReaderState state;
    state.marker_pack_root_directory = proto_filepath.base;
    state.textures = proto_message.textures();

    map<Attribute::UniqueId::UniqueId, Category*> top_level_categories;
    for (int i = 0; i < proto_message.category_size(); i++) {
        Category* category = parse_guildpoint_categories("", proto_message.category(i), marker_categories, parsed_pois, &state);
        top_level_categories[category->menu_id] = category;
    }
    return top_level_categories;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// DESERIALIZE //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// build_category_object
//
// Builds all the hierarchical category object info as long as the category
// has contents that exist in the category filter.
////////////////////////////////////////////////////////////////////////////////
struct MaybeCategory {
    guildpoint::Category category;
    bool is_category;
};
MaybeCategory build_category_objects(
    const Category* category,
    const StringHierarchy& category_filter,
    const std::map<string, std::vector<Parseable*>>& category_to_pois,
    vector<string>* category_vector,
    ProtoWriterState* state
) {
    guildpoint::Category category_proto = category->as_protobuf(state);
    bool has_valid_contents = false;

    vector<guildpoint::Category> categories_to_write;

    for (map<string, Category>::const_iterator it = category->children.begin(); it != category->children.end(); it++) {
        // This is currently a copy operation which is kind expensive
        category_vector->push_back(it->first);

        if (category_filter.in_hierarchy(*category_vector)) {
            MaybeCategory child_category = build_category_objects(
                &it->second,
                category_filter,
                category_to_pois,
                category_vector,
                state
            );

            if (child_category.is_category) {
                has_valid_contents = true;
                category_proto.add_children()->MergeFrom(child_category.category);
            }
        }
        category_vector->pop_back();
    }

    // This is a pretty expensive operation
    string full_category_name = join(*category_vector, ".");
    auto iterator = category_to_pois.find(full_category_name);
    if (iterator != category_to_pois.end()) {
        for (size_t i = 0; i < iterator->second.size(); i++) {
            Parseable* parsed_poi = iterator->second[i];

            if (parsed_poi->classname() == "POI") {
                Icon* icon = dynamic_cast<Icon*>(parsed_poi);
                if (category_filter.in_hierarchy(split(icon->category.category, "."))) {
                    category_proto.add_icon()->MergeFrom(icon->as_protobuf(state));
                    has_valid_contents = true;
                }
            }
            else if (parsed_poi->classname() == "Trail") {
                Trail* trail = dynamic_cast<Trail*>(parsed_poi);
                if (category_filter.in_hierarchy(split(trail->category.category, "."))) {
                    category_proto.add_trail()->MergeFrom(trail->as_protobuf(state));
                    has_valid_contents = true;
                }
            }
            else {
                std::cout << "Unknown type" << std::endl;
            }
        }
    }

    MaybeCategory return_value;
    return_value.category = category_proto;
    return_value.is_category = has_valid_contents;
    return return_value;
}

void proto_post_processing(ProtoWriterState* state, guildpoint::Guildpoint* proto) {
    if (state->textures.size() > 1) {
        // Handle the 0th index null value independently.
        proto->add_textures();

        for (size_t i = 1; i < state->textures.size(); i++) {
            guildpoint::TextureData* texture_data = proto->add_textures();
            texture_data->set_filepath(state->textures[i]->filepath.relative_filepath);
        }
    }
}

void _write_protobuf_file(
    const MarkerPackFile& filepath,
    const StringHierarchy& category_filter,
    const map<string, Category>* marker_categories,
    const std::map<string, std::vector<Parseable*>>& category_to_pois,
    ProtoWriterState* state
) {
    guildpoint::Guildpoint output_message;

    for (map<string, Category>::const_iterator it = marker_categories->begin(); it != marker_categories->end(); it++) {
        string category_name = it->first;
        const Category* category_object = &it->second;

        vector<string> category_vector = {it->first};
        MaybeCategory maybe_category = build_category_objects(
            category_object,
            category_filter,
            category_to_pois,
            &category_vector,
            state
        );

        if (maybe_category.is_category) {
            output_message.add_category()->MergeFrom(maybe_category.category);
        }
    }

    proto_post_processing(state, &output_message);

    stringstream data;
    output_message.SerializeToOstream(&data);
    write_file(filepath, data);
}

std::map<string, std::vector<Parseable*>> construct_category_to_pois_map(const vector<Parseable*>* parsed_pois) {
    std::map<string, std::vector<Parseable*>> category_to_pois;

    for (size_t i = 0; i < parsed_pois->size(); i++) {
        Parseable* parsed_poi = (*parsed_pois)[i];
        if (parsed_poi->classname() == "POI") {
            Icon* icon = dynamic_cast<Icon*>(parsed_poi);
            // TODO(331): This is the wrong place to lowercase() the category and is hiding some crimes elsewhere
            category_to_pois[lowercase(icon->category.category)].push_back(parsed_poi);
        }
        else if (parsed_poi->classname() == "Trail") {
            Trail* trail = dynamic_cast<Trail*>(parsed_poi);
            // TODO(331): This is the wrong place to lowercase() the category and is hiding some crimes elsewhere
            category_to_pois[lowercase(trail->category.category)].push_back(parsed_poi);
        }
        else {
            std::cout << "Unknown type" << std::endl;
        }
    }
    return category_to_pois;
}

void write_protobuf_file(
    const string& marker_pack_root_directory,
    const StringHierarchy& category_filter,
    const map<string, Category>* marker_categories,
    const vector<Parseable*>* parsed_pois
) {
    ProtoWriterState state;
    state.marker_pack_root_directory = marker_pack_root_directory;

    std::map<string, std::vector<Parseable*>> category_to_pois = construct_category_to_pois_map(parsed_pois);

    _write_protobuf_file(
        {state.marker_pack_root_directory, "markers.guildpoint"},
        category_filter,
        marker_categories,
        category_to_pois,
        &state
    );
}

// Write protobuf per map id
void write_protobuf_file_per_map_id(
    const string& marker_pack_root_directory,
    const StringHierarchy& category_filter,
    const map<string, Category>* marker_categories,
    const vector<Parseable*>* parsed_pois
) {
    std::map<int, std::map<string, std::vector<Parseable*>>> mapid_to_category_to_pois;
    ProtoWriterState state;
    state.marker_pack_root_directory = marker_pack_root_directory;

    for (size_t i = 0; i < parsed_pois->size(); i++) {
        Parseable* parsed_poi = (*parsed_pois)[i];
        if (parsed_poi->classname() == "POI") {
            Icon* icon = dynamic_cast<Icon*>(parsed_poi);
            mapid_to_category_to_pois[icon->map_id][icon->category.category].push_back(parsed_poi);
        }
        else if (parsed_poi->classname() == "Trail") {
            Trail* trail = dynamic_cast<Trail*>(parsed_poi);
            mapid_to_category_to_pois[trail->map_id][trail->category.category].push_back(parsed_poi);
        }
        else {
            std::cout << "Unknown type" << std::endl;
        }
    }

    for (auto iterator = mapid_to_category_to_pois.begin(); iterator != mapid_to_category_to_pois.end(); iterator++) {
        _write_protobuf_file(
            {state.marker_pack_root_directory, to_string(iterator->first) + ".guildpoint"},
            category_filter,
            marker_categories,
            iterator->second,
            &state
        );
    }
}
void _category_filter_by_depth(
    const map<string, Category>* marker_categories,
    int target_depth,
    map<string, StringHierarchy>* category_hierarchy,
    vector<string> parent_name,
    vector<string> parent_display_name,
    int current_depth
) {
    for (auto it = marker_categories->begin(); it != marker_categories->end(); it++) {
        vector<string> child_name = parent_name;
        vector<string> child_display_name = parent_display_name;
        child_name.push_back(it->second.name);
        child_display_name.push_back(normalize(it->second.display_name));
        if (target_depth == current_depth) {
            (*category_hierarchy)[join(child_display_name, ".")].add_path(child_name, true);
            continue;
        }
        _category_filter_by_depth(&(it->second.children), target_depth, category_hierarchy, child_name, child_display_name, current_depth + 1);
    }
}

map<string, StringHierarchy> category_filter_by_depth(
    const map<string, Category>* marker_categories,
    const int split_by_category_depth
) {
    map<string, StringHierarchy> category_hierarchy;
    _category_filter_by_depth(marker_categories, split_by_category_depth, &category_hierarchy, {}, {}, 0);
    return category_hierarchy;
}

// Write protobuf per category
void write_protobuf_file_per_category(
    const string& marker_pack_root_directory,
    const int split_by_category_depth,
    const map<string, Category>* marker_categories,
    const vector<Parseable*>* parsed_pois
) {
    ProtoWriterState state;
    state.marker_pack_root_directory = marker_pack_root_directory;

    map<string, StringHierarchy> category_hierarchies = category_filter_by_depth(marker_categories, split_by_category_depth);

    std::map<string, std::vector<Parseable*>> category_to_pois = construct_category_to_pois_map(parsed_pois);

    for (auto iterator = category_hierarchies.begin(); iterator != category_hierarchies.end(); iterator++) {
        _write_protobuf_file(
            {state.marker_pack_root_directory, iterator->first + ".guildpoint"},
            iterator->second,
            marker_categories,
            category_to_pois,
            &state
        );
    }
}
