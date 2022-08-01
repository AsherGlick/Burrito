#pragma once

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


using namespace std;

// #define PARSEABLE_VAR(...) CLASS_PARSEABLE_VAR(Icon, __VA_ARGS__ )
// #define PARSEABLE_SUBVAR(...) CLASS_PARSEABLE_SUBVAR(Icon, __VA_ARGS__ )

class Icon: public Parseable {
	private: 
	
		int achievement_bitmask;
	
		int achievement_id;
	
		float alpha;
	
		bool auto-trigger;
	
		float bounce_delay;
	
		float bounce_duration;
	
		float bounce_height;
	
		bool can_fade;
	
		Category category;
	
		Color color;
	
		string copy_clipboard;
	
		string copy_message;
	
		CullChirality cull_chirality;
	
		float distance_fade_end;
	
		float distance_fade_start;
	
		EulerRotation euler_rotation;
	
		FestivalFilter festival_filter;
	
		UniqueID guid;
	
		bool has_countdown;
	
		float heightoffset;
	
		Category hide_category;
	
		Image icon;
	
		float icon_size;
	
		string info_message;
	
		bool invert_visibility;
	
		int map_display_size;
	
		int map_id;
	
		MapTypeFilter map_type_filter;
	
		int maximum_size_on_screen;
	
		int minimum_size_on_screen;
	
		MountFilter mount_filter;
	
		Position position;
	
		ProfessionFilter profession_filter;
	
		bool render_ingame;
	
		bool render_on_map;
	
		bool render_on_minimap;
	
		ResetBehavior reset_behavior;
	
		float reset_length;
	
		bool scale_on_map_with_zoom;
	
		string schedule;
	
		float schedule_duration;
	
		Category show_category;
	
		SpecializationFilter specialization_filter;
	
		SpeciesFilter species_filter;
	
		Category toggle_category;
	
		string tooltip_description;
	
		string tooltip_name;
	
		float trigger_range;
	
	
	virtual string classname();
};

// #undef PARSEABLE_VAR
// #undef PARSEABLE_SUBVAR