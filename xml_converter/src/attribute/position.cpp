#include "position.hpp"

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;


Position parse_Position(rapidxml::xml_attribute<>* input, vector<string> *errors) {
    vector<string> components = split(string(input->value()), ",");

    Position position;
    if (components.size() == 3) {
        position.x = stof(components[0].c_str());
        position.y = stof(components[1].c_str());
        position.z = stof(components[2].c_str());
    }
    else {
        errors->push_back("invlaid 'x,y,z' rotation " + string(input->value()) + " " + to_string(components.size()));
    }

    return position;
}
