#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include "attribute/marker_category.hpp"
#include "category_gen.hpp"
#include "file_helper.hpp"
#include "icon_gen.hpp"
#include "parseable.hpp"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "string_helper.hpp"
#include "trail_gen.hpp"
#include "waypoint.pb.h"

using namespace std;

void write_xml_file(string xml_filepath, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    ofstream outfile;
    string tab_string;

    outfile.open(xml_filepath, ios::out);

    outfile << "<OverlayData>\n";
    for (const auto& category : *marker_categories) {
        string text;
        for (const auto& s : category.second.as_xml()) {
            text += s;
        }
        outfile << text + "\n";
    }

    outfile << "<POIs>\n";
    for (const auto& parsed_poi : *parsed_pois) {
        string text;
        for (const auto& s : parsed_poi->as_xml()) {
            text += s;
        }
        outfile << text + "\n";
    }
    outfile << "</POIs>\n</OverlayData>\n";

    outfile.close();
}

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

Category* get_category(rapidxml::xml_node<>* node, map<string, Category>* marker_categories, vector<XMLError*>* errors) {
    // TODO: This is a slow linear search, replace with something faster.
    //       maybe use data from already parsed node instead of searching for
    //       the attribute.
    rapidxml::xml_attribute<>* attribute = find_attribute(node, "type");

    if (attribute == 0) {
        // TODO: This error should really be for the entire node not just the name
        errors->push_back(new XMLNodeNameError("No Attribute Named Type", node));
        return nullptr;
    }

    vector<string> split_categories = split(get_attribute_value(attribute), ".");

    if (split_categories.size() == 0) {
        errors->push_back(new XMLAttributeValueError("Empty Type", attribute));
        return nullptr;
    }

    Category* output = nullptr;

    for (unsigned int i = 0; i < split_categories.size(); i++) {
        string category_name = lowercase(split_categories[i]);

        auto category = marker_categories->find(category_name);

        if (category == marker_categories->end()) {
            errors->push_back(new XMLAttributeValueError("Category Not Found \"" + category_name + "\"", attribute));
            return nullptr;
        }

        output = &category->second;

        marker_categories = &output->children;
    }
    return output;
}

////////////////////////////////////////////////////////////////////////////////
// parse_pois
//
// Parse the <POIs> xml block into an in-memory array of Markers.
////////////////////////////////////////////////////////////////////////////////
vector<Parseable*> parse_pois(rapidxml::xml_node<>* root_node, map<string, Category>* marker_categories, vector<XMLError*>* errors, string base_dir) {
    vector<Parseable*> markers;

    for (rapidxml::xml_node<>* node = root_node->first_node(); node; node = node->next_sibling()) {
        if (get_node_name(node) == "POI") {
            Category* default_category = get_category(node, marker_categories, errors);

            Icon* icon = new Icon();

            if (default_category != nullptr) {
                *icon = default_category->default_icon;
            }

            icon->init_from_xml(node, errors);
            markers.push_back(icon);
        }
        else if (get_node_name(node) == "Trail") {
            Category* default_category = get_category(node, marker_categories, errors);

            Trail* trail = new Trail();

            if (default_category != nullptr) {
                *trail = default_category->default_trail;
            }

            trail->init_from_xml(node, errors, base_dir);
            markers.push_back(trail);
        }
        else {
            errors->push_back(new XMLNodeNameError("Unknown POIs node name", node));
        }
    }
    return markers;
}

void parse_marker_categories(rapidxml::xml_node<>* node, map<string, Category>* marker_categories, vector<XMLError*>* errors, int depth = 0) {
    if (get_node_name(node) == "MarkerCategory") {
        string name = lowercase(find_attribute_value(node, "name"));

        Category* this_category = &(*marker_categories)[name];
        this_category->init_from_xml(node, errors);
        for (rapidxml::xml_node<>* child_node = node->first_node(); child_node; child_node = child_node->next_sibling()) {
            parse_marker_categories(child_node, &(this_category->children), errors, depth + 1);
        }
    }
    else {
        errors->push_back(new XMLNodeNameError("Unknown MarkerCategory Tag", node));
    }
}

void parse_waypoint_categories(string full_category_name, ::waypoint::Category proto_category, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
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
// parse_xml_file
//
// A function which parses a single XML file into their corrisponding classes.
////////////////////////////////////////////////////////////////////////////////
void parse_xml_file(string xml_filepath, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    vector<XMLError*> errors;
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* root_node;

    rapidxml::file<> xml_file(xml_filepath.c_str());
    doc.parse<rapidxml::parse_non_destructive | rapidxml::parse_no_data_nodes>(xml_file.data(), xml_filepath.c_str());

    root_node = doc.first_node();
    string base_dir = get_base_dir(xml_filepath);
    // Validate the Root Node
    if (get_node_name(root_node) != "OverlayData") {
        errors.push_back(new XMLNodeNameError("Root node should be of type OverlayData", root_node));
    }
    if (root_node->first_attribute() != nullptr) {
        cout << "Root Node has attributes when it should have none in " << xml_filepath << endl;
    }

    for (rapidxml::xml_node<>* node = root_node->first_node(); node; node = node->next_sibling()) {
        if (get_node_name(node) == "MarkerCategory") {
            parse_marker_categories(node, marker_categories, &errors);
        }
        else if (get_node_name(node) == "POIs") {
            vector<Parseable*> temp_vector = parse_pois(node, marker_categories, &errors, base_dir);
            move(temp_vector.begin(), temp_vector.end(), back_inserter(*parsed_pois));
        }
        else {
            errors.push_back(new XMLNodeNameError("Unknown top-level node name", node));
        }
    }

    for (auto error : errors) {
        error->print_error();
    }
}

void read_protobuf_file(string proto_filepath, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    fstream infile;
    waypoint::Waypoint proto_message;

    infile.open(proto_filepath, ios::in | ios::binary);
    proto_message.ParseFromIstream(&infile);
    for (int i = 0; i < proto_message.category_size(); i++) {
        parse_waypoint_categories("", proto_message.category(i), marker_categories, parsed_pois);
    }
}

bool filename_comp(string a, string b) {
    return lowercase(a) < lowercase(b);
}

vector<string> get_files_by_suffix(string directory, string suffix) {
    vector<string> files;
    DIR* dir = opendir(directory.c_str());
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;
        if (filename != "." && filename != "..") {
            string path = directory + "/" + filename;
            if (entry->d_type == DT_DIR) {
                vector<string> subfiles = get_files_by_suffix(path, suffix);
                // Default: markerpacks have all xml files in the first directory
                for (string subfile : subfiles) {
                    cout << subfile << " found in subfolder" << endl;
                    files.push_back(subfile);
                }
            }
            else if (has_suffix(filename, suffix)) {
                files.push_back(path);
            }
        }
    }
    closedir(dir);
    std::sort(files.begin(), files.end(), filename_comp);
    return files;
}

void move_supplementary_files(string input_directory, string output_directory) {
    DIR* dir = opendir(input_directory.c_str());
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;
        if (filename != "." && filename != "..") {
            string path = input_directory + "/" + filename;
            if (entry->d_type == DT_DIR) {
                string new_directory = output_directory + "/" + filename;
                if (mkdir(new_directory.c_str(), 0700) == -1 && errno != EEXIST) {
                    cout << "Error making " << new_directory << endl;
                    continue;
                }
                move_supplementary_files(path, new_directory);
            }
            else if (has_suffix(filename, ".trl") || has_suffix(filename, ".xml")) {
                continue;
            }
            else {
                // TODO: Only include files that are referenced by the
                // individual markers in order to avoid any unnessecary files
                string new_path = output_directory + "/" + filename;
                copy_file(path, new_path);
            }
        }
    }
}

void read_taco_directory(string directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    vector<string> xml_files = get_files_by_suffix(directory, ".xml");
    for (const string& path : xml_files) {
        parse_xml_file(path, marker_categories, parsed_pois);
    }
}

void write_taco_directory(string directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    // TODO: Exportion of XML Marker Packs File Structure #111
    string xml_filepath = directory + "xml_file.xml";
    write_xml_file(xml_filepath, marker_categories, parsed_pois);
}

void convert_taco_directory(string directory, string output_directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    move_supplementary_files(directory, output_directory);
    read_taco_directory(directory, marker_categories, parsed_pois);
}


////////////////////////////////////////////////////////////////////////////////
// process_data
//
// The universal entrypoint into the xml converter functionality. Both the CLI
// and the library entrypoints direct here to do their actual processing.
////////////////////////////////////////////////////////////////////////////////
void process_data(
    vector<string> input_paths,
    string output_directory
) {
    auto begin = chrono::high_resolution_clock::now();

    vector<Parseable*> parsed_pois;
    map<string, Category> marker_categories;

    try {
        for (size_t i = 0; i < input_paths.size(); i++) {
            cout << input_paths[i] << endl;
            convert_taco_directory(input_paths[i], output_directory, &marker_categories, &parsed_pois);
        }
    }
    catch (const char* msg) {
        cout << msg << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The parse function took " << ms << " milliseconds to run" << endl;

    begin = chrono::high_resolution_clock::now();
    write_xml_file("./export_packs/export.xml", &marker_categories, &parsed_pois);
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The xml write function took " << ms << " milliseconds to run" << endl;

    begin = chrono::high_resolution_clock::now();
    write_protobuf_file(output_directory, &marker_categories, &parsed_pois);
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The protobuf write function took " << ms << " milliseconds to run" << endl;
}


////////////////////////////////////////////////////////////////////////////////
// main
//
// Main is the CLI entrypoint to the xml converter. It handles processing all
// of the command line data into a format the internal functions want to
// receive.
//
// Example usage
//   ./xml_converter --input-path ../packs/marker_pack --output-path ../output_packs
//   ./xml_converter --input-path ../packs/* --output-path ../output_packs
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) {

    vector<string> input_paths;

    // Typically "~/.local/share/godot/app_userdata/Burrito/protobins" for
    // converting from xml markerpacks to internal protobuf files.
    vector<string> output_paths;

    vector<string>* arg_target = &input_paths;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--input-path")) {
            arg_target = &input_paths;
        }
        else if (!strcmp(argv[i], "--output-path")) {
            arg_target = &output_paths;
        }
        else {
            arg_target->push_back(argv[i]);
        }
    }

    cout << input_paths[0] << " " << input_paths.size() << endl;
    cout << output_paths[0] << " " << output_paths.size() << endl;

    process_data(input_paths, output_paths[0]);

    return 0;
}
