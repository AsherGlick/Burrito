#include "../rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "chirality.hpp"


using namespace std;

Chirality parse_Chirality(rapidxml::xml_attribute<>* input, vector<string> *errors) {
	Chirality chirality;
	chirality.chirality = string(input->value());
	return chirality;
}
