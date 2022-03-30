#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include <string>
#include <vector>
#include "profession_filter.hpp"

using namespace std;

ProfessionFilter parse_ProfessionFilter(rapidxml::xml_attribute<>* input, vector<string> *errors) {
	ProfessionFilter profession_filter;
	profession_filter.parse(input, errors);
	return profession_filter;
}