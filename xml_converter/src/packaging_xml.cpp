#include "packaging_xml.hpp"

#include <utility>

#include "hash_helpers.hpp"
#include "rapid_helpers.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "state_structs/xml_reader_state.hpp"
#include "string_helper.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// SERIALIZE ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned int UNKNOWN_CATEGORY_COUNTER = 0;
void parse_marker_categories(
    rapidxml::xml_node<>* node,
    map<string, Category>* marker_categories,
    Category* parent,
    vector<XMLError*>* errors,
    XMLReaderState* state,
    int depth = 0) {
    if (get_node_name(node) == "MarkerCategory") {
        string name;

        rapidxml::xml_attribute<>* name_attribute = find_attribute(node, "name");
        if (name_attribute == 0) {
            // TODO: This error should really be for the entire node not just the name
            errors->push_back(new XMLNodeNameError("Category attribute 'name' is missing so it cannot be properly referenced", node));

            // TODO: Maybe fall back on display name slugification.
            name = "UNKNOWN_CATEGORY_" + to_string(UNKNOWN_CATEGORY_COUNTER);
            UNKNOWN_CATEGORY_COUNTER++;
        }
        else {
            name = lowercase(get_attribute_value(name_attribute));
        }

        if (name == "") {
            errors->push_back(new XMLNodeNameError("Category attribute 'name' is an empty string so it cannot be properly referenced", node));
            // TODO: Maybe fall back on display name slugification.
            name = "UNKNOWN_CATEGORY_" + to_string(UNKNOWN_CATEGORY_COUNTER);
            UNKNOWN_CATEGORY_COUNTER++;
        }

        // Build the new category
        Category* category;

        // Create and initialize a new category if this one does not exist
        auto existing_category_search = marker_categories->find(name);
        if (existing_category_search == marker_categories->end()) {
            category = &(*marker_categories)[name];
            category->parent = parent;
        }
        else {
            category = &existing_category_search->second;
            if (category->parent != parent) {
                errors->push_back(new XMLNodeNameError("Category somehow has a different parent then it used to. This might be a bug in xml_converter", node));
            }
        }

        category->init_from_xml(node, errors, state);

        // If this category does not have an id then create a new one for it
        // based on the hashes of its name and its parents names.
        if (!category->menu_id_is_set) {
            Hash128 new_id;
            new_id.update(name);

            Category* next_node = parent;
            while (next_node != nullptr) {
                new_id.update(next_node->name);
                next_node = next_node->parent;
            }
            category->menu_id = new_id.unique_id();
            category->menu_id_is_set = true;
        }

        for (rapidxml::xml_node<>* child_node = node->first_node(); child_node; child_node = child_node->next_sibling()) {
            parse_marker_categories(child_node, &(category->children), category, errors, state, depth + 1);
        }
    }
    else {
        errors->push_back(new XMLNodeNameError("Unknown MarkerCategory Tag", node));
    }
}

////////////////////////////////////////////////////////////////////////////////
// get_categories
//
// Gets the hirearchy of categories that this node references as its "type" so
// that the defaults of those categories can be used when creating the new node.
////////////////////////////////////////////////////////////////////////////////
vector<Category*> get_categories(
    rapidxml::xml_node<>* node,
    map<string, Category>* marker_categories,
    vector<XMLError*>* errors) {
    vector<Category*> categories;

    rapidxml::xml_attribute<>* attribute = find_attribute(node, "type");

    if (attribute == 0) {
        // TODO: This error should really be for the entire node not just the name
        errors->push_back(new XMLNodeNameError("No Attribute Named Type", node));
        return categories;
    }

    vector<string> split_categories = split(get_attribute_value(attribute), ".");
    if (split_categories.size() == 0) {
        errors->push_back(new XMLAttributeValueError("Empty Type", attribute));
        return categories;
    }

    for (unsigned int i = 0; i < split_categories.size(); i++) {
        string category_name = lowercase(split_categories[i]);

        auto category = marker_categories->find(category_name);
        if (category == marker_categories->end()) {
            errors->push_back(new XMLAttributeValueError("Category Not Found \"" + category_name + "\"", attribute));
            return categories;
        }

        categories.push_back(&category->second);
        marker_categories = &category->second.children;
    }

    return categories;
}

////////////////////////////////////////////////////////////////////////////////
// parse_pois
//
// Parse the <POIs> xml block into an in-memory array of Markers.
////////////////////////////////////////////////////////////////////////////////
vector<Parseable*> parse_pois(rapidxml::xml_node<>* root_node, map<string, Category>* marker_categories, vector<XMLError*>* errors, XMLReaderState* state) {
    vector<Parseable*> markers;

    // A new error array to ignore the parsing erorrs that would have already
    // been caught when the attributes were parsed in the category.
    vector<XMLError*> ignored_errors;

    for (rapidxml::xml_node<>* node = root_node->first_node(); node; node = node->next_sibling()) {
        if (get_node_name(node) == "POI") {
            vector<Category*> categories = get_categories(node, marker_categories, errors);

            Icon* icon = new Icon();

            for (size_t category_index = 0; category_index < categories.size(); category_index++) {
                for (size_t i = 0; i < categories[category_index]->icon_attributes.size(); i++) {
                    icon->init_xml_attribute(
                        categories[category_index]->icon_attributes[i],
                        &ignored_errors,
                        state);
                }
            }

            icon->init_from_xml(node, errors, state);
            markers.push_back(icon);
        }
        else if (get_node_name(node) == "Trail") {
            vector<Category*> categories = get_categories(node, marker_categories, errors);

            Trail* trail = new Trail();

            for (size_t category_index = 0; category_index < categories.size(); category_index++) {
                for (size_t i = 0; i < categories[category_index]->trail_attributes.size(); i++) {
                    trail->init_xml_attribute(
                        categories[category_index]->trail_attributes[i],
                        &ignored_errors,
                        state);
                }
            }

            trail->init_from_xml(node, errors, state);
            markers.push_back(trail);
        }
        else {
            errors->push_back(new XMLNodeNameError("Unknown POIs node name", node));
        }
    }
    return markers;
}

////////////////////////////////////////////////////////////////////////////////
// parse_xml_file
//
// A function which parses a single XML file into their corrisponding classes.
////////////////////////////////////////////////////////////////////////////////
void parse_xml_file(string xml_filepath, const string marker_pack_root_directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    vector<XMLError*> errors;
    rapidxml::xml_document<> doc;
    rapidxml::xml_node<>* root_node;
    XMLReaderState state;
    state.marker_pack_root_directory = marker_pack_root_directory;

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
            parse_marker_categories(node, marker_categories, nullptr, &errors, &state);
        }
        else if (get_node_name(node) == "POIs") {
            vector<Parseable*> temp_vector = parse_pois(node, marker_categories, &errors, &state);
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

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// DESERIALIZE //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void write_xml_file(const string marker_pack_root_directory, map<string, Category>* marker_categories, vector<Parseable*>* parsed_pois) {
    ofstream outfile;
    string tab_string;
    XMLWriterState state;
    state.marker_pack_root_directory = marker_pack_root_directory;

    string xml_filepath = join_file_paths(marker_pack_root_directory, "xml_file.xml");
    outfile.open(xml_filepath, ios::out);

    outfile << "<OverlayData>\n";
    for (const auto& category : *marker_categories) {
        string text;
        for (const auto& s : category.second.as_xml(&state)) {
            text += s;
        }
        outfile << text + "\n";
    }

    outfile << "<POIs>\n";
    for (const auto& parsed_poi : *parsed_pois) {
        string text;
        for (const auto& s : parsed_poi->as_xml(&state)) {
            text += s;
        }
        outfile << text + "\n";
    }
    outfile << "</POIs>\n</OverlayData>\n";

    outfile.close();
}
