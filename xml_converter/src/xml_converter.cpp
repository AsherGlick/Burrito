#include <string.h>

#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <functional>
#include <cstdio>

#include "parseable.hpp"
#include "trail_gen.hpp"
#include "icon_gen.hpp"
#include "category_gen.hpp"
#include "attribute/float.hpp"
#include "string_helper.hpp"
#include "rapid_helpers.hpp"

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

using namespace std;






bool has_suffix(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

Category* get_category(rapidxml::xml_node<>* node, map<string, Category>* marker_categories, vector<XMLError*>* errors) {
    rapidxml::xml_attribute<>* attribute = find_attribute(node, "type");
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
vector<Parseable> parse_pois(rapidxml::xml_node<>* root_node, map<string, Category>* marker_categories, vector<XMLError*>* errors) {
    vector<Parseable> markers;


    for (rapidxml::xml_node<>* node = root_node->first_node(); node; node = node->next_sibling()) {

        if (get_node_name(node) == "POI") {
            Category* default_category = get_category(node, marker_categories, errors);

            Icon icon;

            if (default_category != nullptr) {
                icon = default_category->default_icon;
            }

            icon.init_from_xml(node, errors);
            markers.push_back(icon);
        }
        else if (get_node_name(node) == "Trail") {
            Category* default_category = get_category(node, marker_categories, errors);

            Trail trail;

            if (default_category != nullptr) {
                trail = default_category->default_trail;
            }

            trail.init_from_xml(node, errors);
            markers.push_back(trail);
        }
        else {
            errors->push_back(new XMLNodeNameError("Unknown POIs node name", node));
        }
    }
    return markers;
}


void parse_marker_categories(rapidxml::xml_node<>* node, map<string, Category>* marker_categories, vector<XMLError*>* errors, int depth=0) {
    if (get_node_name(node) == "MarkerCategory") {
        string name = lowercase(find_attribute_value(node, "name"));

        Category* this_category = &(*marker_categories)[name];
        this_category->init_from_xml(node, errors);

        for (rapidxml::xml_node<>* child_node = node->first_node(); child_node; child_node = child_node->next_sibling()) {
            parse_marker_categories(child_node, &(this_category->children), errors, depth +1);
        }
    }
    else {
        errors->push_back(new XMLNodeNameError("Unknown MarkerCategory Tag", node));
    }
}


////////////////////////////////////////////////////////////////////////////////
// parse_xml_file
//
// A function which parses a single XML file into their corrisponding classes.
////////////////////////////////////////////////////////////////////////////////
void parse_xml_file(string xml_filepath, map<string, Category>* marker_categories) {
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
            parse_pois(node, marker_categories, &errors);
        }
        else {
            errors.push_back(new XMLNodeNameError("Unknown top-level node name", node));
        }
    }

    for (auto error : errors) {
        error->print_error();
    }
}




bool filename_comp(string a, string b){
    return lowercase(a) < lowercase(b);
}


vector<string> get_xml_files(string directory) {
    vector<string> files;
    vector<string> subfolders;

    for (const auto & entry : filesystem::directory_iterator(directory)) {
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

    for (const string & subfolder : subfolders ) {
        vector<string> subfiles = get_xml_files(subfolder);
        files.insert( files.end(), subfiles.begin(), subfiles.end() );
    }

    return files;
}

void convert_taco_directory(string directory) {
    map<string, Category> marker_categories;

    vector<string> xml_files = get_xml_files(directory);

    for (const string & path : xml_files) {
        parse_xml_file(path, &marker_categories);
    }
}

int main() {
    

    for (const auto & entry : filesystem::directory_iterator("./packs")) {
        string path = entry.path();
        if (entry.is_directory()) {
            convert_taco_directory(path);
        }
        else {
            continue;
        }
    }

    return 0;
}
