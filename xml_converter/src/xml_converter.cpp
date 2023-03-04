#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
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
#include "attribute/trail_data.hpp"
#include "category_gen.hpp"
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
// Iterates through all children of a Category and removes those that do not
// have a nodes that belongs to it.
////////////////////////////////////////////////////////////////////////////////
void remove_proto_child(waypoint::Category* proto_category, set<string> categories_to_retain, string parent_name) {
    int keep = 0;
    for (int i = 0; i < proto_category->children_size(); i++) {
        string name = parent_name + "." + proto_category->children(i).name();
        auto pos = categories_to_retain.find(lowercase(name));
        if (pos != categories_to_retain.end()) {
            if (keep < i) {
                proto_category->mutable_children()->SwapElements(i, keep);
                if (proto_category->children(i).children_size() >= 0) {
                    for (int j = 0; j < proto_category->children_size(); j++) {
                        remove_proto_child(proto_category->mutable_children(j), categories_to_retain, name);
                    }
                }
            }
            ++keep;
        }
    }
    proto_category->mutable_children()->DeleteSubrange(keep, proto_category->children_size() - keep);
}

int get_trail_map_id(Trail* trail) {
    ifstream trail_data_file;
    string trail_path = trail->trail_data.base_dir + "/" + trail->trail_data.trail_data;
    trail_data_file.open(trail_path, ios::in | ios::binary);

    char version[4];
    trail_data_file.read(version, 4);

    char map_id[4];

    trail_data_file.read(map_id, 4);
    uint32_t map_num = *reinterpret_cast<uint32_t*>(map_id);
    trail_data_file.close();
    return map_num;
}

void read_trail_data_file(Trail* trail, waypoint::Trail* proto_trail) {
    ifstream trail_data_file;
    string trail_path = trail->trail_data.base_dir + "/" + trail->trail_data.trail_data;
    trail_data_file.open(trail_path, ios::in | ios::binary);

    char version[4];
    trail_data_file.read(version, 4);

    char map_id[4];

    trail_data_file.read(map_id, 4);
    uint32_t map_num = *reinterpret_cast<uint32_t*>(map_id);
    proto_trail->set_map_id(map_num);

    vector<float> points_x;
    vector<float> points_y;
    vector<float> points_z;

    while (trail_data_file.tellg() > 0) {
        char point_x[4];
        trail_data_file.read(point_x, 4);
        points_x.push_back(*reinterpret_cast<float*>(point_x));
        char point_y[4];
        trail_data_file.read(point_y, 4);
        points_y.push_back(*reinterpret_cast<float*>(point_y));
        char point_z[4];
        trail_data_file.read(point_z, 4);
        points_z.push_back(*reinterpret_cast<float*>(point_z));
    }

    if (points_x.size() != points_z.size()) {
        cout << "Unexpected number of bits in trail file " << trail_path << endl;
    }

    *proto_trail->mutable_trail_data()->mutable_points_x() = {points_x.begin(), points_x.end()};
    *proto_trail->mutable_trail_data()->mutable_points_y() = {points_y.begin(), points_y.end()};
    *proto_trail->mutable_trail_data()->mutable_points_z() = {points_z.begin(), points_z.end()};

    trail_data_file.close();
}

void write_protobuf_file(string proto_directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    // Creates a Waypoint message that contains all categories
    waypoint::Waypoint all_categories;
    for (const auto& category : *marker_categories) {
        waypoint::Category proto_category = category.second.as_protobuf();
        all_categories.add_category()->CopyFrom(proto_category);
    }

    waypoint::Waypoint proto_pois;
    // Collects a set of map ids from Icon and Trail data
    std::set<int> map_ids;
    ofstream trail_data_file;
    for (const auto& parsed_poi : *parsed_pois) {
        if (parsed_poi->classname() == "POI") {
            Icon* icon = dynamic_cast<Icon*>(parsed_poi);
            map_ids.insert(icon->map_id);
        }
        else if (parsed_poi->classname() == "Trail") {
            Trail* trail = dynamic_cast<Trail*>(parsed_poi);
            map_ids.insert(trail->map_id);
        }
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
                    output_message.add_icon()->MergeFrom(icon->as_protobuf());
                }
            }
            else if (parsed_poi->classname() == "Trail") {
                Trail* trail = dynamic_cast<Trail*>(parsed_poi);
                if (trail->map_id == map_id) {
                    populate_categories_to_retain(trail->category.category, &categories_to_retain);
                    waypoint::Trail proto_trail = trail->as_protobuf();
                    read_trail_data_file(trail, &proto_trail);
                    output_message.add_trail()->MergeFrom(proto_trail);
                }
            }
        }
        // In the XML, MarkerCategories have a tree hierarchy while POIS have a
        // flat hierarchy. This is preserved in the protobuf for ease of
        // translation. We are doing a removal instead of an insertion because
        // each parent category contains the data for all of its children. It
        // would be impractical to include all of the data from each category
        // except for the children and then iterating over all the children.
        // This pruning method is slower but ensures that information is kept.
        for (int i = 0; i < output_message.category_size(); i++) {
            remove_proto_child(output_message.mutable_category(i), categories_to_retain, output_message.category(i).name());
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
vector<Parseable*> parse_pois(string base_dir, rapidxml::xml_node<>* root_node, map<string, Category>* marker_categories, vector<XMLError*>* errors) {
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

            trail->init_from_xml(node, errors);
            if (trail->trail_data_is_set) {
                trail->trail_data.base_dir = base_dir;
                trail->map_id = get_trail_map_id(trail);
            }
            markers.push_back(trail);
        }
        else {
            errors->push_back(new XMLNodeNameError("Unknown POIs node name", node));
        }
    }
    return markers;
}

vector<Parseable*> parse_proto_pois(waypoint::Waypoint proto_message) {
    vector<Parseable*> markers;

    for (int i = 0; i < proto_message.icon_size(); i++) {
        Icon* icon = new Icon();
        icon->parse_protobuf(proto_message.icon(i));
        markers.push_back(icon);
    }
    for (int i = 0; i < proto_message.trail_size(); i++) {
        Trail* trail = new Trail();
        trail->parse_protobuf(proto_message.trail(i));
        markers.push_back(trail);
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
void parse_proto_marker_categories(::waypoint::Category proto_category, map<string, Category>* marker_categories) {
    string name = proto_category.name();
    Category* this_category = &(*marker_categories)[name];
    this_category->parse_protobuf(proto_category);
    for (int i = 0; i < proto_category.children_size(); i++) {
        parse_proto_marker_categories(proto_category.children(i), &(this_category->children));
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
            vector<Parseable*> temp_vector = parse_pois(base_dir, node, marker_categories, &errors);
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
        parse_proto_marker_categories(proto_message.category(i), marker_categories);
    }
    vector<Parseable*> temp_vector = parse_proto_pois(proto_message);
    move(temp_vector.begin(), temp_vector.end(), back_inserter(*parsed_pois));
}

bool filename_comp(string a, string b) {
    return lowercase(a) < lowercase(b);
}

vector<string> get_xml_files(string directory, string data_directory) {
    vector<string> files;
    DIR* dir = opendir(directory.c_str());
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;
        if (filename != "." && filename != "..") {
            string path = directory + "/" + filename;
            if (entry->d_type == DT_DIR) {
                string new_folder = data_directory + "/" + filename;
                if (mkdir(new_folder.c_str(), 0700) == -1 && errno != EEXIST) {
                    cout << "Error making " << new_folder << endl;
                    continue;
                }
                vector<string> subfiles = get_xml_files(path, new_folder);
                for (string subfile : subfiles) {
                    cout << subfile << endl;
                    files.push_back(subfile);
                }
            }
            else if (entry->d_type == DT_REG) {
                if (has_suffix(filename, ".xml")) {
                    files.push_back(path);
                }
                else if (has_suffix(filename, ".trl") == false) {
                    string new_path = data_directory + "/" + filename;
                    ifstream infile(path, ios::binary);
                    ofstream outfile(new_path, ios::binary);
                    outfile << infile.rdbuf();
                }
            }
        }
    }
    closedir(dir);
    std::sort(files.begin(), files.end(), filename_comp);
    return files;
}

void convert_taco_directory(string directory, string data_directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    vector<string> xml_files = get_xml_files(directory, data_directory);
    for (const string& path : xml_files) {
        parse_xml_file(path, marker_categories, parsed_pois);
    }
}

void test_proto() {
    waypoint::Category testcategory;
    testcategory.set_display_name("TEST");
    if (testcategory.name() != "") {
        cout << "Error in test_proto" << endl;
        throw std::error_code();
    }
    string output = testcategory.display_name();
    if (output != "TEST") {
        cout << "Error in test_proto" << endl;
        throw std::error_code();
    }
}

int main() {
    auto begin = chrono::high_resolution_clock::now();

    vector<Parseable*> parsed_pois;
    map<string, Category> marker_categories;
    test_proto();

    string output_directory = "./protobins";

    if (mkdir(output_directory.c_str(), 0700) == -1 && errno != EEXIST) {
        cout << "Error making ./protobins/" << endl;
        throw std::error_code();
    }

    string input_directory = "./packs";
    vector<string> marker_packs;

    DIR* dir = opendir(input_directory.c_str());
    struct dirent* entry = readdir(dir);
    while ((entry = readdir(dir)) != NULL) {
        string filename = entry->d_name;
        if (entry->d_type == DT_DIR && filename != "." && filename != "..") {
            string path = input_directory + "/" + filename;
            marker_packs.push_back(path);
            cout << path << endl;
            convert_taco_directory(path, output_directory, &marker_categories, &parsed_pois);
        }
    }
    closedir(dir);

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

    ////////////////////////////////////////////////////////////////////////////
    // This section tests that the protobuf file can be parsed back to xml
    ////////////////////////////////////////////////////////////////////////////
    parsed_pois.clear();
    marker_categories.clear();

    begin = chrono::high_resolution_clock::now();
    read_protobuf_file("./protobins/50.data", &marker_categories, &parsed_pois);
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The protobuf read function took " << ms << " milliseconds to run" << endl;

    begin = chrono::high_resolution_clock::now();
    write_xml_file("./protobins/50.xml", &marker_categories, &parsed_pois);
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The xml write function took " << ms << " milliseconds to run" << endl;

    return 0;
}
