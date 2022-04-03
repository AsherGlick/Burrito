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

#define PARSEABLE_VAR(...) CLASS_PARSEABLE_VAR(Trail, __VA_ARGS__ )
#define PARSEABLE_SUBVAR(...) CLASS_PARSEABLE_SUBVAR(Trail, __VA_ARGS__ )

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
class Trail: public Parseable {
 public:
    // https://blishhud.com/docs/markers/attributes/achievement
    PARSEABLE_VAR(achievement_id, int, "AchievementId")
    PARSEABLE_VAR(achievement_bit, int, "AchievementBit")

    // https://blishhud.com/docs/markers/attributes/alpha
    PARSEABLE_VAR(alpha, float, "Alpha")

    // https://blishhud.com/docs/markers/attributes/animspeed
    PARSEABLE_VAR(animation_speed, float, "AnimSpeed")

    // https://blishhud.com/docs/markers/attributes/canfade
    PARSEABLE_VAR(can_fade, bool, "CanFade")

    // https://blishhud.com/docs/markers/attributes/color
    PARSEABLE_VAR(color, Color, "Color")

    // https://blishhud.com/docs/markers/attributes/cull
    PARSEABLE_VAR(cull, Chirality, "Cull")

    // https://blishhud.com/docs/markers/attributes/fade
    PARSEABLE_VAR(fade_near, float, "FadeNear")
    PARSEABLE_VAR(fade_far, float, "FadeFar")

    // https://blishhud.com/docs/markers/attributes/festival
    PARSEABLE_VAR(festival_filter, FestivalFilter, "Festival")

    // https://blishhud.com/docs/markers/attributes/iswall
    PARSEABLE_VAR(is_wall, bool, "IsWall")

    // https://blishhud.com/docs/markers/attributes/maptype
    PARSEABLE_VAR(map_type, MapTypeFilter, "MapType")

    // https://blishhud.com/docs/markers/attributes/mount
    PARSEABLE_VAR(mount, MountFilter, "Mount")

    // https://blishhud.com/docs/markers/attributes/profession
    PARSEABLE_VAR(profession_filter, ProfessionFilter, "Profession")

    // https://blishhud.com/docs/markers/attributes/race
    PARSEABLE_VAR(race_filter, RaceFilter, "Race")

    // https://blishhud.com/docs/markers/attributes/texture
    PARSEABLE_VAR(texture, string, "Texture")

    // https://blishhud.com/docs/markers/attributes/trailscale
    PARSEABLE_VAR(trail_scale, float, "TrailScale")

    // https://blishhud.com/docs/markers/attributes/type
    PARSEABLE_VAR(category, string, "Type")

    // https://blishhud.com/docs/markers/attributes/visibility
    PARSEABLE_VAR(visible_on_minimap, bool, "MinimapVisibility")
    PARSEABLE_VAR(visible_on_map, bool, "MapVisibility")
    PARSEABLE_VAR(visible_ingame, bool, "IngameVisibility")

    // Currently Uncatagorized by BlishHUD
    PARSEABLE_VAR(trail_data, string, "TrailData")
    PARSEABLE_VAR(guid, string, "GUID")  // https://blishhud.com/docs/markers/attributes/guid (but this is only valid for markers it seems)
    PARSEABLE_VAR(map_display_size, int, "MapDisplaySize")


    virtual string classname();
};

#undef PARSEABLE_VAR
#undef PARSEABLE_SUBVAR
