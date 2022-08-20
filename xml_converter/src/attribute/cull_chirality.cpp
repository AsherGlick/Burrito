#include "cull_chirality.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

CullChirality parse_cull_chirality(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
	CullChirality cull_chirality;
	if (get_attribute_value(input) == "None") {
		cull_chirality = CullChirality::none;
	}
	else if (get_attribute_value(input) == "Clockwise") {
		cull_chirality = CullChirality::clockwise;
	}
	else if (get_attribute_value(input) == "CounterClockwise") {
		cull_chirality = CullChirality::counter_clockwise;
	}
	else {errors->push_back(new XMLAttributeValueError("Found a value that was not in the Enum", input));
        cull_chirality = CullChirality::none;
    }
	return cull_chirality;
}