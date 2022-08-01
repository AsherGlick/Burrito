#pragma once

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


using namespace std;

// #define PARSEABLE_VAR(...) CLASS_PARSEABLE_VAR(Icon, __VA_ARGS__ )
// #define PARSEABLE_SUBVAR(...) CLASS_PARSEABLE_SUBVAR(Icon, __VA_ARGS__ )

class Trail: public Parseable {
	private: 
	
		int achievement_bitmask;
	
		int achievement_id;
	
		float alpha;
	
		float animation_speed;
	
		bool can_fade;
	
		Category category;
	
		Color color;
	
		CullChirality cull_chirality;
	
		float distance_fade_end;
	
		float distance_fade_start;
	
		FestivalFilter festival_filter;
	
		UniqueID guid;
	
		bool is_wall;
	
		int map_id;
	
		MapTypeFilter map_type_filter;
	
		MountFilter mount_filter;
	
		ProfessionFilter profession_filter;
	
		bool render_ingame;
	
		bool render_on_map;
	
		bool render_on_minimap;
	
		string schedule;
	
		float schedule_duration;
	
		SpecializationFilter specialization_filter;
	
		SpeciesFilter species_filter;
	
		Image texture;
	
		TrailData trail_data;
	
		TrailDataMapId trail_data_map_id;
	
		float trail_scale;
	
	
	virtual string classname();
};

// #undef PARSEABLE_VAR
// #undef PARSEABLE_SUBVAR