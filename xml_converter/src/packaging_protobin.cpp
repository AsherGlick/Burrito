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
// Adds the name of a category and all of it's parents to a set
// eg.
// {
//     "mypath",
//     "mypath.easypath",
//     "mypath.easypath.trail",
//     "mypath.hardpath",
//     "mypath.hardpath.trail",
// }
////////////////////////////////////////////////////////////////////////////////
void populate_categories_to_retain(string category_name, set<string>* categories_to_retain) {
    string name;
    vector<string> split_categories = split(category_name, ".");
    for (unsigned int j = 0; j < split_categories.size(); j++) {
        name += split_categories[j];
        categories_to_retain->insert(name);
        name += ".";
    }
}

////////////////////////////////////////////////////////////////////////////////
// Iterates through all children of a Category and removes the categories and
// POIs that do not belong on a particular map
////////////////////////////////////////////////////////////////////////////////
void remove_waypoint_elements(waypoint::Category* proto_category, set<string> categories_to_retain, string parent_name, int map_id) {
    int keep = 0;
    for (int i = 0; i < proto_category->icon_size(); i++) {
        if (proto_category->icon(i).map_id() == map_id) {
            if (keep < i) {
                proto_category->mutable_icon()->SwapElements(i, keep);
            }
            ++keep;
        }
    }
    proto_category->mutable_icon()->DeleteSubrange(keep, proto_category->icon_size() - keep);

    keep = 0;
    for (int i = 0; i < proto_category->trail_size(); i++) {
        if (proto_category->trail(i).map_id() == map_id) {
            if (keep < i) {
                proto_category->mutable_trail()->SwapElements(i, keep);
            }
            ++keep;
        }
    }
    proto_category->mutable_trail()->DeleteSubrange(keep, proto_category->trail_size() - keep);

    keep = 0;
    for (int i = 0; i < proto_category->children_size(); i++) {
        string name = parent_name + "." + proto_category->children(i).name();
        auto pos = categories_to_retain.find(name);
        if (pos != categories_to_retain.end()) {
            remove_waypoint_elements(proto_category->mutable_children(i), categories_to_retain, name, map_id);
            if (keep < i) {
                proto_category->mutable_children()->SwapElements(i, keep);
            }
            ++keep;
        }
    }
    proto_category->mutable_children()->DeleteSubrange(keep, proto_category->children_size() - keep);
}

void write_protobuf_file(string proto_directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    waypoint::Waypoint proto_pois;
    // Collects a set of map ids from Icon and Trail data
    std::set<int> map_ids;
    ofstream trail_data_file;
    map<string, vector<Parseable*>> map_of_pois;
    for (const auto& parsed_poi : *parsed_pois) {
        if (parsed_poi->classname() == "POI") {
            Icon* icon = dynamic_cast<Icon*>(parsed_poi);
            map_ids.insert(icon->map_id);
            map_of_pois[icon->category.category].push_back(icon);
        }
        else if (parsed_poi->classname() == "Trail") {
            Trail* trail = dynamic_cast<Trail*>(parsed_poi);
            map_ids.insert(trail->map_id);
            map_of_pois[trail->category.category].push_back(trail);
        }
    }

    // Creates a Waypoint message that contains all categories
    waypoint::Waypoint all_categories;
    for (const auto& category : *marker_categories) {
        waypoint::Category proto_category = category.second.as_protobuf("", &map_of_pois);
        all_categories.add_category()->CopyFrom(proto_category);
    }

    waypoint::Waypoint output_message;
    std::set<string> categories_to_retain;
    for (int map_id : map_ids) {
        ofstream outfile;
        string output_filepath = proto_directory + "/" + to_string(map_id) + ".data";
        outfile.open(output_filepath, ios::out | ios::binary);
        output_message.MergeFrom(all_categories);

        for (const auto& parsed_poi : *parsed_pois) {
            if (parsed_poi->classname() == "POI") {
                Icon* icon = dynamic_cast<Icon*>(parsed_poi);
                if (icon->map_id == map_id) {
                    populate_categories_to_retain(icon->category.category, &categories_to_retain);
                }
            }
            else if (parsed_poi->classname() == "Trail") {
                Trail* trail = dynamic_cast<Trail*>(parsed_poi);
                if (trail->map_id == map_id) {
                    populate_categories_to_retain(trail->category.category, &categories_to_retain);
                }
            }
        }
        // In the XML, MarkerCategories have a tree hierarchy while POIS have a
        // flat hierarchy. In Waypoint, POIs are elements of the category they
        // belong to. We are removing instead of inserting because
        // each parent category contains the data for all of its children. It
        // would be impractical to include all of the data from each category
        // except for the children and then iterating over all the children.
        // This pruning method is slower but ensures that information is kept.
        for (int i = 0; i < output_message.category_size(); i++) {
            remove_waypoint_elements(output_message.mutable_category(i), categories_to_retain, output_message.category(i).name(), map_id);
            if (output_message.mutable_category(i)->children_size() == 0) {
                output_message.mutable_category(i)->Clear();
            }
        }
        output_message.SerializeToOstream(&outfile);
        outfile.close();
        output_message.Clear();
        categories_to_retain.clear();
    }
}
