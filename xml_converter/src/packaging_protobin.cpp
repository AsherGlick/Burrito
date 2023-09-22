#include "packaging_protobin.hpp"

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "category_gen.hpp"
#include "parseable.hpp"
#include "string_helper.hpp"
#include "waypoint.pb.h"
#include "string_hierarchy.hpp"

#include <google/protobuf/text_format.h> // For TextProtos

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SERIALIZE ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void parse_waypoint_categories(
    string full_category_name,
    ::waypoint::Category proto_category,
    map<string, Category>* marker_categories,
    vector<Parseable*>* parsed_pois) {
    full_category_name += proto_category.name();
    Category* this_category = &(*marker_categories)[full_category_name];
    this_category->parse_protobuf(proto_category);

    for (int i = 0; i < proto_category.icon_size(); i++) {
        Icon* icon = new Icon();
        icon->parse_protobuf(proto_category.icon(i));
        // TODO: The field category in Icon is being deprciated
        // This overwrites any icon.category with its position in the heirarchy
        icon->category.category = full_category_name;
        parsed_pois->push_back(icon);
    }
    for (int i = 0; i < proto_category.trail_size(); i++) {
        Trail* trail = new Trail();
        trail->parse_protobuf(proto_category.trail(i));
        // TODO: The field category in Trail is being deprciated
        // This overwrites any trail.category with its position in the heirarchy
        trail->category.category = full_category_name;
        parsed_pois->push_back(trail);
    }

    for (int i = 0; i < proto_category.children_size(); i++) {
        parse_waypoint_categories(full_category_name + ".", proto_category.children(i), &(this_category->children), parsed_pois);
    }
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void read_protobuf_file(string proto_filepath, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    fstream infile;
    waypoint::Waypoint proto_message;

    infile.open(proto_filepath, ios::in | ios::binary);
    proto_message.ParseFromIstream(&infile);
    for (int i = 0; i < proto_message.category_size(); i++) {
        parse_waypoint_categories("", proto_message.category(i), marker_categories, parsed_pois);
    }
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
    waypoint::Category category;
    bool is_category;
};
MaybeCategory build_category_objects(
    const Category* category,
    const StringHierarchy &category_filter,
    const std::map<string, std::vector<Parseable*>> &category_to_pois,
    vector<string> &category_vector
) {
    waypoint::Category category_proto = category->as_protobuf();
    bool has_valid_contents = false;

    vector<waypoint::Category> categories_to_write;

    for (map<string, Category>::const_iterator it = category->children.begin(); it != category->children.end(); it++) {

        // This is currently a copy operation which is kind expensive
        category_vector.push_back(it->first);

        if (category_filter.in_hierarchy(category_vector)) {
            MaybeCategory child_category = build_category_objects(
                &it->second,
                category_filter,
                category_to_pois,
                category_vector
            );

            if (child_category.is_category) {
                has_valid_contents = true;
                category_proto.add_children()->MergeFrom(child_category.category);
            }
        }
        category_vector.pop_back();
    }

    // This is a pretty expensive operation
    string full_category_name = join(category_vector, ".");
    auto iterator = category_to_pois.find(full_category_name);
    if (iterator != category_to_pois.end()) {
        for (size_t i = 0; i < iterator->second.size(); i++) {
            Parseable* parsed_poi = iterator->second[i];

            if (parsed_poi->classname() == "POI") {
                Icon* icon = dynamic_cast<Icon*>(parsed_poi);
                if (category_filter.in_hierarchy(split(icon->category.category,"."))) {
                    category_proto.add_icon()->MergeFrom(icon->as_protobuf());
                    has_valid_contents = true;
                }
            }
            else if (parsed_poi->classname() == "Trail") {
                Trail* trail = dynamic_cast<Trail*>(parsed_poi);
                if (category_filter.in_hierarchy(split(trail->category.category,"."))) {
                    category_proto.add_trail()->MergeFrom(trail->as_protobuf());
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

void write_protobuf_file(
    const string &filepath,
    const StringHierarchy &category_filter,
    const map<string, Category>* marker_categories,
    const vector<Parseable*>* parsed_pois
) {
    // TODO: call _write_protobuf_file
}

void _write_protobuf_file(
    const string &filepath,
    const StringHierarchy &category_filter,
    const map<string, Category>* marker_categories,
    const std::map<string, std::vector<Parseable*>> &category_to_pois
){
    ofstream outfile;
    outfile.open(filepath, ios::out | ios::binary);

    waypoint::Waypoint output_message;

    for (map<string, Category>::const_iterator it = marker_categories->begin(); it != marker_categories->end(); it++) {
        string category_name = it->first;
        const Category* category_object = &it->second;

        vector<string> category_vector = {it->first};
        MaybeCategory maybe_category = build_category_objects(
            category_object,
            category_filter,
            category_to_pois,
            category_vector
        );

        if (maybe_category.is_category) {
            output_message.add_category()->MergeFrom(maybe_category.category);
        }
    }

    output_message.SerializeToOstream(&outfile);
    outfile.close();
}

// Write protobuf per map id
void write_protobuf_file_per_map_id(
    const string &proto_directory,
    const StringHierarchy &category_filter,
    const map<string, Category>* marker_categories,
    const vector<Parseable*>* parsed_pois
) {

    std::map<int, std::map<string, std::vector<Parseable*>>> mapid_to_category_to_pois;

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

        string output_filepath = proto_directory + "/" + to_string(iterator->first) + ".data";

        _write_protobuf_file(
            output_filepath,
            category_filter,
            marker_categories,
            iterator->second
        );
    }
}

