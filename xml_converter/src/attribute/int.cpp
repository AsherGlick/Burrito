#include "int.hpp"

#include <stdexcept>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"

using namespace std;

int parse_int(rapidxml::xml_attribute<>* input, vector<string> *errors) {
    try {
        return stoi(input->value());
    }
    catch(std::invalid_argument const& exception) {
        errors->push_back("Invalid integer value '" + string(input->value()) + "'");
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
