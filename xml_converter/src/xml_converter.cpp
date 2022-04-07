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

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

using namespace std;






bool has_suffix(std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

Category* get_category(string name, map<string, Category>* marker_categories, vector<string>* errors) {
    vector<string> split_categories = split(name, ".");

    if (split_categories.size() == 0) {
        errors->push_back("Empty Type " + name);
        return nullptr;
    }

    Category* output = nullptr;

    for (unsigned int i = 0; i < split_categories.size(); i++) {
        string category_name = split_categories[i];

        auto category = marker_categories->find(category_name);

        if (category == marker_categories->end()) {
            errors->push_back("Category Not Found " + category_name + " " + name);
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
vector<Parseable> parse_pois(rapidxml::xml_node<>* root_node, map<string, Category>* marker_categories, string filename, vector<string>* errors) {
    vector<Parseable> markers;


    for (rapidxml::xml_node<>* node = root_node->first_node(); node; node = node->next_sibling()) {

        if (string(node->name()) == "POI") {
            Category* default_category = get_category(node->first_attribute("type", 0, false)->value(), marker_categories, errors);

            Icon poi;
            poi.init_from_xml(node, errors);
            markers.push_back(poi);
        }
        else if (string(node->name()) == "Trail") {
            Category* default_category = get_category(node->first_attribute("type", 0, false)->value(), marker_categories, errors);

            Trail trail;
            trail.init_from_xml(node, errors);
            markers.push_back(trail);
        }
        else {
            cout << "Unknown POIs node name \"" << node->name() << "\" " << filename << endl;
        }
    }
    return markers;
}


void parse_marker_categories(rapidxml::xml_node<>* node, map<string, Category>* marker_categories, vector<string>* errors, int depth=0) {
    if (string(node->name()) == "MarkerCategory") {
        string name = node->first_attribute("name", 0, false)->value();

        Category* this_category = &(*marker_categories)[name];
        this_category->init_from_xml(node, errors);

        for (rapidxml::xml_node<>* child_node = node->first_node(); child_node; child_node = child_node->next_sibling()) {
            parse_marker_categories(child_node, &(this_category->children), errors, depth +1);
        }
    }
    else {
        errors->push_back("unknown maker category tag " + string(node->name()));
    }



}


////////////////////////////////////////////////////////////////////////////////
// parse_xml_file
//
// A function which parses a single XML file into their corrisponding classes.
////////////////////////////////////////////////////////////////////////////////
void parse_xml_file(string xml_filepath, map<string, Category>* marker_categories) {
    vector<string> errors;

    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* root_node;

    ifstream inputFile(xml_filepath);
    vector<char> buffer((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);

    root_node = doc.first_node();


    // Validate the Root Node
    if (string(root_node->name()) != "OverlayData") {
        cout << "Root Node is \"" << root_node->name() <<  "\" not \"OverlayData\" in " << xml_filepath << endl;
    }
    if (root_node->first_attribute() != nullptr) {
        cout << "Root Node has attributes when it should have none in " << xml_filepath << endl;
    }



    for (rapidxml::xml_node<>* node = root_node->first_node(); node; node = node->next_sibling()) {
        if (string(node->name()) == "MarkerCategory") {
            parse_marker_categories(node, marker_categories, &errors);
        }
        else if (string(node->name()) == "POIs") {
            parse_pois(node, marker_categories, xml_filepath, &errors);
        }
        else {
            cout << "Unknown top-level node name " << node->name() << endl;
        }
    }

    if (errors.size() > 0) {
        cout << xml_filepath << endl;
        for (auto error : errors) {
            cout << error << endl;
        }
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
