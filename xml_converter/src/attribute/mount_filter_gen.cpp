#include "mount_filter_gen.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

MountFilter parse_mount_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
    MountFilter mount_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");  
    mount_filter.raptor = false;  
    mount_filter.springer = false;  
    mount_filter.skimmer = false;  
    mount_filter.jackal = false;  
    mount_filter.griffon = false;  
    mount_filter.roller_beetle = false;  
    mount_filter.warclaw = false;  
    mount_filter.skyscale = false;  
    mount_filter.skiff = false;  
    mount_filter.seige_turtle = false;
   
    for (string flag_value : flag_values) {
        string normalized_flag_value = normalize(flag_value);
        if (normalized_flag_value == "raptor") {
            mount_filter.raptor = true; 
        }
        else if (normalized_flag_value == "springer") {
            mount_filter.springer = true; 
        }
        else if (normalized_flag_value == "skimmer") {
            mount_filter.skimmer = true; 
        }
        else if (normalized_flag_value == "jackal") {
            mount_filter.jackal = true; 
        }
        else if (normalized_flag_value == "griffon") {
            mount_filter.griffon = true; 
        }
        else if (normalized_flag_value == "rollerbeetle") {
            mount_filter.roller_beetle = true; 
        }
        else if (normalized_flag_value == "warclaw") {
            mount_filter.warclaw = true; 
        }
        else if (normalized_flag_value == "skyscale") {
            mount_filter.skyscale = true; 
        }
        else if (normalized_flag_value == "skiff") {
            mount_filter.skiff = true; 
        }
        else if (normalized_flag_value == "seigeturtle") {
            mount_filter.seige_turtle = true; 
        }
        else {
            errors->push_back(new XMLAttributeValueError("Invalid Filter for MountFilter. Found " + flag_value, input));
            continue;
        }
    }
    return mount_filter;
}

string stringify_mount_filter(MountFilter attribute_value){
    string output = "";
    if (attribute_value.raptor == true){
        output = output + "raptor";
    }
    if (attribute_value.springer == true){
        output = output + "springer";
    }
    if (attribute_value.skimmer == true){
        output = output + "skimmer";
    }
    if (attribute_value.jackal == true){
        output = output + "jackal";
    }
    if (attribute_value.griffon == true){
        output = output + "griffon";
    }
    if (attribute_value.roller_beetle == true){
        output = output + "rollerbeetle";
    }
    if (attribute_value.warclaw == true){
        output = output + "warclaw";
    }
    if (attribute_value.skyscale == true){
        output = output + "skyscale";
    }
    if (attribute_value.skiff == true){
        output = output + "skiff";
    }
    if (attribute_value.seige_turtle == true){
        output = output + "seigeturtle";
    }
    return output;
}