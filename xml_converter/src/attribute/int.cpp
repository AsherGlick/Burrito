#include "int.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

int parse_int(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors) {
    try {
        return stoi(get_attribute_value(input));
    }
    catch(std::invalid_argument const& exception) {
        errors->push_back(new XMLAttributeValueError("Invalid integer value", input));
        return 0;
    }
    // catch(std::out_of_range const& exception) {
    //     std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
    //     const long long ll {std::stoll(s, &pos)};
    //     std::cout << "std::stoll('" << s << "'): " << ll << "; pos: " << pos << '\n';
    // }
}

int init_int_attribute() {
    return 0;
}
