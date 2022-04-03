#pragma once

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include <string>
#include <vector>
#include "filter.hpp"


using namespace std;

#define FILTER_ITEM(...) CLASS_FILTER_ITEM(ProfessionFilter, __VA_ARGS__ )

class ProfessionFilter: public Filter {
public:
	FILTER_ITEM(elementalist, "elementalist")
	FILTER_ITEM(engineer, "engineer")
	FILTER_ITEM(guardian, "guardian")
	FILTER_ITEM(mesmer, "mesmer")
	FILTER_ITEM(necromancer, "necromancer")
	FILTER_ITEM(ranger, "ranger")
	FILTER_ITEM(revenant, "revenant")
	FILTER_ITEM(thief, "thief")
	FILTER_ITEM(warrior, "warrior")

    virtual string classname() { return "ProfessionFilter"; }
};

#undef FILTER_ITEM

ProfessionFilter parse_ProfessionFilter(rapidxml::xml_attribute<>* input, vector<string> *errors);
