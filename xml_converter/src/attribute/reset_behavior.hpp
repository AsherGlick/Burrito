#pragma once

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

class ResetBehavior {
 public:
    string reset_behavior;

    virtual string classname() { return "ResetBehavior"; };
};

ResetBehavior parse_ResetBehavior(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors);