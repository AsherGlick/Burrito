#include "rapid_helpers.hpp"
#include <iostream>
#include <string>
using namespace std;

string find_attribute_value(rapidxml::xml_node<>* node, string attribute_name) {
    auto attribute = node->first_attribute(attribute_name.data(), attribute_name.size(), false);
    
    return string(attribute->value(), attribute->value_size());
}

rapidxml::xml_attribute<>* find_attribute(rapidxml::xml_node<>* node, string attribute_name) {
    return node->first_attribute(attribute_name.data(), attribute_name.size(), false);
}

string get_attribute_name(rapidxml::xml_attribute<>* attribute) {
    return string(attribute->name(), attribute->name_size());
}

string get_attribute_value(rapidxml::xml_attribute<>* attribute) {
    return string(attribute->value(), attribute->value_size());
}

string get_node_name(rapidxml::xml_node<>* node) {
    return string(node->name(), node->name_size());
}



struct TextPosition {
    uint line_number;
    uint column_number;
    char* line_start;
    uint line_length;
};


TextPosition get_line_number(char* source, char* start_index, string filepath) {
    uint newline_count = 0;
    uint column_number = 0;
    char* line_start = nullptr;

    char* index = start_index;

    while (index >= source) {
        if (*index == '\n') {
            if (line_start == nullptr) {
                line_start = index+1;
            }

            newline_count++;
        }
        if (line_start == nullptr) {
            column_number++;
        }
        index--;
    }

    uint line_length = column_number;
    index = start_index;
    while (*index != '\n' && *index != '\0') {
        index++;
        line_length++;
    }

    return TextPosition{
        newline_count,
        column_number,
        line_start,
        line_length
    };
}



string replace_tabs(string input){
    string tab = "\t"; //tab
    string spaces = "    "; //four spaces

    auto iterator = input.find(tab);
    while (iterator != string::npos)
    {
        input.replace(iterator, tab.size(), spaces);
        iterator = input.find(tab);
    }
    return input;
}
void print_generic_error(string error_message, char* source, string filepath, char* start_index, uint length) {

    string BOLD_COLOR = "\033[1m";
    string RED_COLOR = "\033[31;1m";
    string DEFAULT_COLOR = "\033[0m";

    cout << BOLD_COLOR << "Error: " << DEFAULT_COLOR << error_message << " " << filepath << endl;


    if (start_index < source) {
        cout << "some weird fuckery" << endl;
        return;
    }



    TextPosition text_position = get_line_number(source, start_index, filepath);
    string line_number_string = to_string(text_position.line_number);
    string padding_string = string(line_number_string.length(), ' ');


    string prefix = replace_tabs(string(text_position.line_start, text_position.column_number-1));
    string value = replace_tabs(string(start_index, length));

    uint suffix_length = text_position.line_length - length - text_position.column_number;
    string suffix = string(start_index+length, suffix_length);


    string prefix_padding = string(prefix.length(), ' ');
    string value_markers = string(value.length(), '^');


    cout << line_number_string << " |" << prefix << RED_COLOR << value << DEFAULT_COLOR << suffix << endl;
    cout << padding_string << " |" << prefix_padding << RED_COLOR << value_markers << DEFAULT_COLOR << endl << endl;
}




XMLAttributeNameError::XMLAttributeNameError(string message, rapidxml::xml_attribute<>* attribute) {
    this->message = message;
    this->attribute = attribute;
}
void XMLAttributeNameError::print_error(char* source, string filepath) {
    print_generic_error(this->message, source, filepath, this->attribute->name(), this->attribute->name_size());
}

XMLAttributeValueError::XMLAttributeValueError(string message, rapidxml::xml_attribute<>* attribute) {
    this->message = message;
    this->attribute = attribute;
}
void XMLAttributeValueError::print_error(char* source, string filepath) {
    print_generic_error(this->message, source, filepath, this->attribute->value(), this->attribute->value_size());
}


XMLNodeNameError::XMLNodeNameError(string message, rapidxml::xml_node<>* node) {
    this->message = message;
    this->node = node;
}
void XMLNodeNameError::print_error(char* source, string filepath){
    print_generic_error(this->message, source, filepath, this->node->name(), this->node->name_size());
}
