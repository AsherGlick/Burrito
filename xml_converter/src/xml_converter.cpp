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
#include "trail.hpp"
#include "icon.hpp"
#include "category.hpp"
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

Category* get_category(rapidxml::xml_attribute<>* name, map<string, Category>* marker_categories, vector<XMLError*>* errors) {
    vector<string> split_categories = split(get_attribute_value(name), ".");

    if (split_categories.size() == 0) {
        errors->push_back(new XMLAttributeValueError("Empty Type", name));
        return nullptr;
    }

    Category* output = nullptr;

    for (unsigned int i = 0; i < split_categories.size(); i++) {
        string category_name = split_categories[i];

        auto category = marker_categories->find(category_name);

        if (category == marker_categories->end()) {
            errors->push_back(new XMLAttributeValueError("Category " + category_name + " Not Found ", name));
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
// Parse the <POIs> xml block into an in memory array of Markers.
////////////////////////////////////////////////////////////////////////////////
vector<Parseable> parse_pois(rapidxml::xml_node<>* root_node, map<string, Category>* marker_categories, string filename, vector<XMLError*>* errors) {
    vector<Parseable> markers;


    for (rapidxml::xml_node<>* node = root_node->first_node(); node; node = node->next_sibling()) {

        if (get_node_name(node) == "POI") {
            Category* default_category = get_category(find_attribute(node, "type"), marker_categories, errors);

            Icon poi;
            poi.init_from_xml(node, errors);
            markers.push_back(poi);
        }
        else if (get_node_name(node) == "Trail") {
            Category* default_category = get_category(find_attribute(node, "type"), marker_categories, errors);

            Trail trail;
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
        string name = find_attribute_value(node, "name");

        Category* this_category = &(*marker_categories)[name];
        this_category->init_from_xml(node, errors);

        for (rapidxml::xml_node<>* child_node = node->first_node(); child_node; child_node = child_node->next_sibling()) {
            parse_marker_categories(child_node, &(this_category->children), errors, depth +1);
        }
    }
    else {
        errors->push_back(new XMLNodeNameError("Unknown MarkerCategory tag", node));
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
    doc.parse<rapidxml::parse_non_destructive>(xml_file.data());

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
            parse_pois(node, marker_categories, xml_filepath, &errors);
        }
        else {
            errors.push_back(new XMLNodeNameError("Unknown top-level node name", node));
        }
    }

    for (auto error : errors) {
        error->print_error(xml_file.data(), xml_filepath);
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
    convert_taco_directory("./packs/tw_ALL_IN_ONE");
    convert_taco_directory("./packs/TehsTrails");
    convert_taco_directory("./packs/MoW");
    convert_taco_directory("./packs/Hero.Blish.Pack");
    convert_taco_directory("./packs/GW2 TacO ReActif EN External");
    convert_taco_directory("./packs/602fd903dad6efast_TacO_pack_001");

    return 0;
}
