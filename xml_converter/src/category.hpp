#include "rapidxml-1.13/rapidxml.hpp"

#include <vector>
#include <string>
#include "icon.hpp"
#include "trail.hpp"
#include "parseable.hpp"
using namespace std;

class Cateogry: public Parseable {
public:
	// https://blishhud.com/docs/markers/attributes/defaulttoggle
	PARSEABLE_VAR(default_toggle, bool, "DefaultToggle")

	// https://blishhud.com/docs/markers/attributes/displayname
	PARSEABLE_VAR(display_name, string, "DisplayName")

	// https://blishhud.com/docs/markers/attributes/isseparator
	PARSEABLE_VAR(is_separator, bool, "IsSeparator")

	// Not Documented on blishhud
	PARSEABLE_VAR(name, string, "Name")



	vector<Cateogry> children;
	Icon default_icon;
	Trail default_trail;


	virtual string classname();
	void init_from_xml(rapidxml::xml_node<>* node, vector<string> *errors);
	// virtual bool init_xml_attribute(rapidxml::xml_attribute<>* attribute, vector<string> *errors);
};

