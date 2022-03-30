#include "rapidxml-1.13/rapidxml.hpp"

#include <vector>
#include <string>
#include "icon.hpp"
#include "trail.hpp"
using namespace std;

class Cateogry {
public:
	// https://blishhud.com/docs/markers/attributes/defaulttoggle
	bool default_toggle;

	// https://blishhud.com/docs/markers/attributes/displayname
	string display_name;

	// https://blishhud.com/docs/markers/attributes/isseparator
	bool is_seperator;



	vector<Cateogry> children;
	Icon default_icon;
	Trail default_trail;


	virtual string classname();
	void init_from_xml(rapidxml::xml_node<>* node, vector<string> *errors);
	virtual bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<string> *errors);
};

