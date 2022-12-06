#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
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

void write_protobuf_file(string proto_filepath, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    ofstream outfile;
    waypoint::Waypoint proto_message;

    outfile.open(proto_filepath, ios::out | ios_base::binary);
    for (const auto& category : *marker_categories) {
        waypoint::Category proto_category;
        proto_category = category.second.as_protobuf(proto_category);
        proto_message.add_category()->CopyFrom(proto_category);
    }

    for (const auto& parsed_poi : *parsed_pois) {
        if (parsed_poi->classname() == "POI"){
            waypoint::Icon proto_Icon;
            proto_Icon = parsed_poi->as_protobuf(proto_Icon);
            proto_message.add_icon()->CopyFrom(proto_Icon);
        }
        if (parsed_poi->classname() == "Trail"){
            waypoint::Trail proto_Trail;
            proto_Trail = parsed_poi->as_protobuf(proto_Trail);
            proto_message.add_trail()->CopyFrom(proto_Trail);
        }
    }
    proto_message.SerializeToOstream(&outfile);
    outfile.close();
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
void parse_proto_marker_categories(::waypoint::Category proto_category, map<string, Category>* marker_categories){
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

void read_protobuf_file(string proto_filepath, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois){
    fstream infile;
    waypoint::Waypoint proto_message;

    infile.open(proto_filepath, ios::in | ios_base::binary);
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

    for (const auto& entry : filesystem::directory_iterator(directory)) {
        string path = entry.path();
        if (entry.is_directory()) {
            subfolders.push_back(path);
        }
        else {
            if (has_suffix(path, ".xml")) {
                files.push_back(path);
            }
        }
    }

    std::sort(files.begin(), files.end(), filename_comp);
    std::sort(subfolders.begin(), subfolders.end(), filename_comp);

    for (const string& subfolder : subfolders) {
        vector<string> subfiles = get_xml_files(subfolder);
        files.insert(files.end(), subfiles.begin(), subfiles.end());
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
    if (testcategory.name() != ""){
    cout << "Error in test_proto"  << endl; 
    }   
    string output = testcategory.display_name();
    if (output != "TEST") {
        cout << "Error in test_proto" << endl;
    }
}

int main() {
    auto begin = chrono::high_resolution_clock::now();

    vector<Parseable*> parsed_pois;
    map<string, Category> marker_categories;
    test_proto();

    for (const auto& entry : filesystem::directory_iterator("../packs")) {
        string path = entry.path();

        if (entry.is_directory()) {
            convert_taco_directory(path, &marker_categories, &parsed_pois);
        }
        else {
            continue;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << "The parse function took " << ms << " milliseconds to run" << endl;

//     begin = chrono::high_resolution_clock::now();
//     write_xml_file("../export_packs/export.xml", &marker_categories, &parsed_pois);
//     end = chrono::high_resolution_clock::now();
//     dur = end - begin;
//     ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
//     cout << "The xml write function took " << ms << " milliseconds to run" << endl;

//     begin = chrono::high_resolution_clock::now();
//     write_protobuf_file("../export_packs/export.data", &marker_categories, &parsed_pois);
//     end = chrono::high_resolution_clock::now();
//     dur = end - begin;
//     ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
//     cout << "The protobuf write function took " << ms << " milliseconds to run" << endl;


// ////////////////////////////////////////////////////////////////////////////////////////
// /// This section tests that the protobuf file can be parsed back to xml
// ////////////////////////////////////////////////////////////////////////////////////////

//     parsed_pois.clear();
//     marker_categories.clear();

//     begin = chrono::high_resolution_clock::now();
//     read_protobuf_file("../export_packs/export.data", &marker_categories, &parsed_pois);
//     end = chrono::high_resolution_clock::now();
//     dur = end - begin;
//     ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
//     cout << "The protobuf read function took " << ms << " milliseconds to run" << endl;

//     begin = chrono::high_resolution_clock::now();
//     write_xml_file("../export_packs/export2.xml", &marker_categories, &parsed_pois);
//     end = chrono::high_resolution_clock::now();
//     dur = end - begin;
//     ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
//     cout << "The xml write function took " << ms << " milliseconds to run" << endl;

    return 0;
}
