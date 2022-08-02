#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"
{% if page == "Category": %}#include <map>
#include "icon.hpp"
#include "trail.hpp"
{% elif page == "Trail": %}#include <string.h>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string_view>
#include "rapidxml-1.13/rapidxml_print.hpp"
{% endif %}
{%for include in includelist%}#include "{{include}}.hpp"
{% endfor %}
using namespace std;

class {{page}}: public Parseable {
	private: {% for fieldrow in fieldrows: %}
		{{fieldrow[1]}} {{fieldrow[0]}};{% endfor %}
		{% if page == "Category": %}map<string, Category> children;
    	Icon default_icon;
    	Trail default_trail;{% endif %}
		virtual string classname();
};