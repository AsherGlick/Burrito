#pragma once
{% if page == "Icon": %}
#include <string>
#include <vector>
#include "attribute/bool.hpp"
#include "attribute/chirality.hpp"
#include "attribute/color.hpp"
#include "attribute/euler_angle.hpp"
#include "attribute/festival_filter.hpp"
#include "attribute/float.hpp"
#include "attribute/image.hpp"
#include "attribute/int.hpp"
#include "attribute/map_type_filter.hpp"
#include "attribute/mount_filter.hpp"
#include "attribute/position.hpp"
#include "attribute/profession_filter.hpp"
#include "attribute/race_filter.hpp"
#include "attribute/specialization_filter.hpp"
#include "attribute/string.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "string_helper.hpp"
{% elif page == "Category": %}
#include <map>
#include <string>
#include <vector>

#include "icon.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "trail.hpp"
{% elif page == "Trail": %}
#include <string.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "attribute/bool.hpp"
#include "attribute/chirality.hpp"
#include "attribute/color.hpp"
#include "attribute/festival_filter.hpp"
#include "attribute/float.hpp"
#include "attribute/image.hpp"
#include "attribute/int.hpp"
#include "attribute/map_type_filter.hpp"
#include "attribute/mount_filter.hpp"
#include "attribute/profession_filter.hpp"
#include "attribute/race_filter.hpp"
#include "attribute/string.hpp"
#include "parseable.hpp"
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
{% endif %}

using namespace std;

// #define PARSEABLE_VAR(...) CLASS_PARSEABLE_VAR(Icon, __VA_ARGS__ )
// #define PARSEABLE_SUBVAR(...) CLASS_PARSEABLE_SUBVAR(Icon, __VA_ARGS__ )

class {{page}}: public Parseable {
	private: 
	{% for fieldrow in fieldrows: %}
		{{fieldrow[1]}} {{fieldrow[0]}};
	{% endfor %}
	{% if page == "Category": %}
	  map<string, Category> children;
    	Icon default_icon;
    	Trail default_trail;
    {% endif %}
	virtual string classname();
};

// #undef PARSEABLE_VAR
// #undef PARSEABLE_SUBVAR