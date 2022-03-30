#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

#include <google/protobuf/generated_message_reflection.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>
#include <string.h>
#include <functional>
#include <cstdio>
using namespace std;

#include "parseable.hpp"
#include "trail.hpp"
#include "icon.hpp"
#include "category.hpp"

#include "attribute/float.hpp"




bool has_suffix (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}



////////////////////////////////////////////////////////////////////////////////
// parse_pois
//
// Parse the <POIs> xml block into an in memory array of Markers.
////////////////////////////////////////////////////////////////////////////////
vector<Parseable> parse_pois(rapidxml::xml_node<>* root_node, string filename, vector<string>* errors) {
	vector<Parseable> markers;

	for (rapidxml::xml_node<>* node = root_node->first_node(); node; node = node->next_sibling()) {
		if (string(node->name()) == "POI") {
			Icon poi;
			poi.init_from_xml(node, errors);
			markers.push_back(poi);
		}
		else if (string(node->name()) == "Trail") {
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


void parse_marker_categories(rapidxml::xml_node<>* root_node, vector<string>* errors) {
	for (rapidxml::xml_node<>* node = root_node->first_node(); node; node = node->next_sibling()) {
		if (string(node->name()) == "MarkerCategory") {
			Cateogry marker_category;
			marker_category.init_from_xml(node, errors);
			parse_marker_categories(node, errors);
		}
		else {
			errors->push_back("unknown maker category tag " + string(node->name()));
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// parse_xml_file
//
// 
////////////////////////////////////////////////////////////////////////////////
void parse_xml_file(string xml_filepath) {

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
			// parse_marker_categories(node, &errors);
		}
		else if (string(node->name()) == "POIs") {
			parse_pois(node, xml_filepath, &errors);
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



void convert_taco_directory(string directory) {
	for (const auto & entry : filesystem::directory_iterator(directory)) {
		string path = entry.path();
		if (has_suffix(path, ".xml")) {
			parse_xml_file(path);
		}
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
