#include <dirent.h>
#include <errno.h>
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

bool has_suffix(std::string const& fullString, std::string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

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
// Adds the name of a category and all of its children to a setgit 
// eg.
// {
//     "mypath",
//     "mypath.easypath",
//     "mypath.easypath.trail",
//     "mypath.hardpath",
//     "mypath.hardpath.trail",
// }
////////////////////////////////////////////////////////////////////////////////
void add_children_names(waypoint::Category proto_category, set<string>* proto_categories, string parent_name) {
    string name = parent_name + "." + lowercase(proto_category.name());
    proto_categories->insert(name);
    for (int i = 0; i < proto_category.children_size(); i++) {
        add_children_names(proto_category.children(i), proto_categories, name);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Adds the name of a category and all of it's parents to a set
////////////////////////////////////////////////////////////////////////////////
void add_parent_names(string category_name, set<string> proto_categories, set<string>* category_includes) {
    auto category = proto_categories.find(category_name);
    if (category == proto_categories.end()) {
        cout << "Unknown MarkerCategory " << category_name << endl;
    }
    else {
        string name;
        vector<string> split_categories = split(category_name, ".");
        for (unsigned int j = 0; j < split_categories.size(); j++) {
            name += split_categories[j];
            category_includes->insert(name);
            name += ".";
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Iterates through all children of a Category and removes those that do not
// have POIs with corresponding names
////////////////////////////////////////////////////////////////////////////////
void remove_proto_child(waypoint::Category* proto_category, set<string> category_includes, string parent_name) {
    int keep = 0;
    for (int i = 0; i < proto_category->children_size(); i++) {
        string name = parent_name + "." + proto_category->children(i).name();
        auto pos = category_includes.find(lowercase(name));
        if (pos != category_includes.end()) {
            if (keep < i) {
                proto_category->mutable_children()->SwapElements(i, keep);
                if (proto_category->children(i).children_size() >= 0) {
                    for (int j = 0; j < proto_category->children_size(); j++) {
                        remove_proto_child(proto_category->mutable_children(j), category_includes, name);
                    }
                }
            }
            ++keep;
        }
    }
    proto_category->mutable_children()->DeleteSubrange(keep, proto_category->children_size() - keep);
}

void write_protobuf_file(string proto_filepath, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    if (mkdir(proto_filepath.c_str(), 0700) == -1 && errno != EEXIST) {
        cout << "Error making ./protobins" << endl;
        throw std::error_code();
    }
    // Collects a set of category names and their children
    waypoint::Waypoint all_categories;
    std::set<string> proto_categories;
    for (const auto& category : *marker_categories) {
        waypoint::Category proto_category = category.second.as_protobuf();
        all_categories.add_category()->CopyFrom(proto_category);
        add_children_names(proto_category, &proto_categories, proto_category.name());
    }
    // Collects a set of map ids from Icon and Trail data
    std::set<int> map_ids;
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
    std::set<string> category_includes;
    for (int map_id : map_ids) {
        ofstream outfile;
        string output_filepath = proto_filepath + "/" + to_string(map_id) + ".data";
        outfile.open(output_filepath, ios::out | ios::binary);
        output_message.MergeFrom(all_categories);

        for (const auto& parsed_poi : *parsed_pois) {
            if (parsed_poi->classname() == "POI") {
                Icon* icon = dynamic_cast<Icon*>(parsed_poi);
                if (icon->map_id == map_id) {
                    add_parent_names(icon->category.category, proto_categories, &category_includes);
                    output_message.add_icon()->MergeFrom(icon->as_protobuf());
                }
            }
            else if (parsed_poi->classname() == "Trail") {
                Trail* trail = dynamic_cast<Trail*>(parsed_poi);
                if (trail->map_id == map_id) {
                    add_parent_names(trail->category.category, proto_categories, &category_includes);
                    output_message.add_trail()->MergeFrom(trail->as_protobuf());
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
            remove_proto_child(output_message.mutable_category(i), category_includes, output_message.category(i).name());
            if (output_message.mutable_category(i)->children_size() == 0) {
                output_message.mutable_category(i)->Clear();
            }
        }
        output_message.SerializeToOstream(&outfile);
        outfile.close();
        output_message.Clear();
        category_includes.clear();
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
vector<Parseable*> parse_pois(rapidxml::xml_node<>* root_node, map<string, Category>* marker_categories, vector<XMLError*>* errors) {
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
            vector<Parseable*> temp_vector = parse_pois(node, marker_categories, &errors);
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

vector<string> get_xml_files(string directory) {
    vector<string> files;
    vector<string> subfolders;
    string path;
    DIR* dir = opendir(directory.c_str());
    struct dirent* entry = readdir(dir);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR && has_suffix(directory, ".") == false) {
            path = directory + "/";
            path += entry->d_name;
            subfolders.push_back(path);
        }
        else if (has_suffix(entry->d_name, ".xml")) {
            path = directory + "/";
            path += entry->d_name;
            files.push_back(path);
        }
        else {
            continue;
        }
    }
    closedir(dir);

    std::sort(files.begin(), files.end(), filename_comp);
    std::sort(subfolders.begin(), subfolders.end(), filename_comp);

    for (const std::string& subfolder : subfolders) {
        if (has_suffix(subfolder, ".") == false && has_suffix(subfolder, "..") == false) {
            vector<string> subfiles = get_xml_files(subfolder);
            files.insert(files.end(), subfiles.begin(), subfiles.end());
        }
    }
    return files;
}

void convert_taco_directory(string directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    vector<string> xml_files = get_xml_files(directory);

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

    string directory = "./packs";
    DIR* dir = opendir(directory.c_str());
    struct dirent* entry = readdir(dir);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            string path = directory + "/";
            path += entry->d_name;
            convert_taco_directory(path, &marker_categories, &parsed_pois);
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
    write_protobuf_file("./protobins", &marker_categories, &parsed_pois);
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
    read_protobuf_file("./protobins/15.data", &marker_categories, &parsed_pois);
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The protobuf read function took " << ms << " milliseconds to run" << endl;

    begin = chrono::high_resolution_clock::now();
    write_xml_file("./protobins/15.xml", &marker_categories, &parsed_pois);
    end = chrono::high_resolution_clock::now();
    dur = end - begin;
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The xml write function took " << ms << " milliseconds to run" << endl;

    return 0;
}
