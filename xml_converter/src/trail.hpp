#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include <string>
#include <vector>
#include "parseable.hpp"
#include <string.h>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string_view>
#include "rapidxml-1.13/rapidxml_print.hpp"

#include "bool.hpp"
#include "category.hpp"
#include "color.hpp"
#include "cull_chirality.hpp"
#include "festival_filter.hpp"
#include "float.hpp"
#include "image.hpp"
#include "int.hpp"
#include "map_type_filter.hpp"
#include "mount_filter.hpp"
#include "profession_filter.hpp"
#include "specialization_filter.hpp"
#include "species_filter.hpp"
#include "string.hpp"
#include "traildata.hpp"
#include "traildatamapid.hpp"
#include "uniqueid.hpp"

using namespace std;

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