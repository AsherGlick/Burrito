#include "rapid_helpers.hpp"

#include <sys/types.h>

#include <iostream>
#include <string>

#include "rapidxml-1.13/rapidxml.hpp"

using namespace std;

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

////////////////////////////////////////////////////////////////////////////////
// TextPosition
//
// The return value for get_line_number(). Contains several pieces of useful
// information about the location of a particular character and its context.
////////////////////////////////////////////////////////////////////////////////
struct TextPosition {
    uint line_number;
    uint column_number;
    char* line_start;
    uint line_length;
};

////////////////////////////////////////////////////////////////////////////////
// get_line_number
//
// Gets several useful pieces of information about a particular token that
// starts at `start_index` and is within `source`.
////////////////////////////////////////////////////////////////////////////////
TextPosition get_line_number(char* source, char* start_index) {
    uint newline_count = 0;
    uint column_number = 0;
    char* line_start = nullptr;

    char* index = start_index;

    while (index >= source) {
        if (*index == '\n') {
            if (line_start == nullptr) {
                line_start = index + 1;
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
        newline_count + 1,
        column_number,
        line_start,
        line_length};
}

////////////////////////////////////////////////////////////////////////////////
// replace_tabs
//
// This function creates a new string with all the tabs replaced with four
// spaces. This is done to normalize the size of the strings when printed.
////////////////////////////////////////////////////////////////////////////////
string replace_tabs(string input) {
    string tab = "\t";
    string spaces = "    ";

    auto iterator = input.find(tab);
    while (iterator != string::npos) {
        input.replace(iterator, tab.size(), spaces);
        iterator = input.find(tab);
    }
    return input;
}

////////////////////////////////////////////////////////////////////////////////
// print_generic_error
//
// Prints out a formatted error message taking in some non-specific values
// in order to do so.
////////////////////////////////////////////////////////////////////////////////
string generate_generic_error(string error_message, char* source, string filepath, char* start_index, uint length) {
    string BOLD_COLOR = "\033[1m";
    string RED_COLOR = "\033[31;1m";
    string DEFAULT_COLOR = "\033[0m";

    string output;

    // Print out the first line of the error containing the message and the
    // filename, leaving the location hint for later in this function.
    output = RED_COLOR + "Error: " + DEFAULT_COLOR + BOLD_COLOR + error_message + DEFAULT_COLOR + "\n" + filepath + "\n";

    // Sanity check to see if we are definitely not inside the string.
    if (start_index < source) {
        cout << "Unable to Identify Error in source" << endl;
        return output;
    }

    // Calculate the row, column, and some other values for the token.
    TextPosition text_position = get_line_number(source, start_index);

    // Calculate the left-hand column text.
    string line_number_string = to_string(text_position.line_number);
    string padding_string = string(line_number_string.length(), ' ');

    // Calculate the strings for the row of XML that should be shown to the
    // user, splitting out the value which will be colored.
    string prefix = replace_tabs(string(text_position.line_start, text_position.column_number - 1));
    string value = replace_tabs(string(start_index, length));
    uint suffix_length = text_position.line_length - length - text_position.column_number;
    string suffix = string(start_index + length, suffix_length);

    // Calculate the strings that position and indicate where the error is for
    // displays that do not use color.
    string prefix_padding = string(prefix.length(), ' ');
    string value_markers = string(value.length(), '^');

    // Display the formatted lines to the user.
    return output + line_number_string + " |" + prefix + RED_COLOR + value + DEFAULT_COLOR + suffix + "\n" +
           padding_string + " |" + prefix_padding + RED_COLOR + value_markers + DEFAULT_COLOR + "\n";
}

void XMLError::print_error() {
    cout << this->error_message << endl;
}

////////////////////////////////////////////////////////////////////////////////
// Implementation of the constructor for the XMLAttributeNameError subclass.
////////////////////////////////////////////////////////////////////////////////
XMLAttributeNameError::XMLAttributeNameError(string message, rapidxml::xml_attribute<>* attribute) {
    this->error_message = generate_generic_error(
        message,
        attribute->document()->source,
        attribute->document()->source_name,
        attribute->name(),
        attribute->name_size());
}

////////////////////////////////////////////////////////////////////////////////
// Implementation of the constructor for the XMLAttributeValueError subclass.
////////////////////////////////////////////////////////////////////////////////
XMLAttributeValueError::XMLAttributeValueError(string message, rapidxml::xml_attribute<>* attribute) {
    this->error_message = generate_generic_error(
        message,
        attribute->document()->source,
        attribute->document()->source_name,
        attribute->value(),
        attribute->value_size());
}

////////////////////////////////////////////////////////////////////////////////
// Implementation of the constructor for the XMLNodeNameError subclass.
////////////////////////////////////////////////////////////////////////////////
XMLNodeNameError::XMLNodeNameError(string message, rapidxml::xml_node<>* node) {
    this->error_message = generate_generic_error(
        message,
        node->document()->source,
        node->document()->source_name,
        node->name(),
        node->name_size());
}
