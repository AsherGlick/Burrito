#pragma once

#include <string>
#include <vector>

#include "attribute/bool.hpp"
#include "attribute/chirality.hpp"
#include "attribute/color.hpp"
#include "attribute/euler_angle.hpp"
#include "attribute/festival_filter.hpp"
#include "attribute/float.hpp"
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



#define PARSEABLE_VAR(...) CLASS_PARSEABLE_VAR(Icon, __VA_ARGS__ )
#define PARSEABLE_SUBVAR(...) CLASS_PARSEABLE_SUBVAR(Icon, __VA_ARGS__ )

class Icon: public Parseable {
 private:
    // https://blishhud.com/docs/markers/attributes/achievement
    PARSEABLE_VAR(achievement_id, int, "AchievementId")
    PARSEABLE_VAR(achievement_bit, int, "AchievementBit")

    // https://blishhud.com/docs/markers/attributes/alpha
    PARSEABLE_VAR(alpha, float, "Alpha")

    // https://blishhud.com/docs/markers/attributes/autotrigger
    PARSEABLE_VAR(auto_trigger, bool, "AutoTrigger")

    // https://blishhud.com/docs/markers/attributes/behavior
    PARSEABLE_VAR(behavior, int, "Behavior")

    // https://blishhud.com/docs/markers/attributes/bounce
    PARSEABLE_VAR(bounce_height, float, "BounceHeight")
    PARSEABLE_VAR(bounce_duration, float, "BounceDuration")
    PARSEABLE_VAR(bounce_delay, float, "BounceDelay")

    // https://blishhud.com/docs/markers/attributes/canfade
    PARSEABLE_VAR(can_fade, bool, "CanFade")

    // https://blishhud.com/docs/markers/attributes/color
    PARSEABLE_VAR(color, Color, "Color")

    // https://blishhud.com/docs/markers/attributes/copy
    PARSEABLE_VAR(copy, string, "Copy")
    PARSEABLE_VAR(copy_message, string, "CopyMessage")

    // https://blishhud.com/docs/markers/attributes/cull
    PARSEABLE_VAR(cull, Chirality, "Cull")

    // https://blishhud.com/docs/markers/attributes/fade
    PARSEABLE_VAR(fade_near, float, "FadeNear")
    PARSEABLE_VAR(fade_far, float, "FadeFar")

    // https://blishhud.com/docs/markers/attributes/festival
    PARSEABLE_VAR(festival_filter, FestivalFilter, "Festival")

    // https://blishhud.com/docs/markers/attributes/guid
    PARSEABLE_VAR(guid, string, "GUID")

    // https://blishhud.com/docs/markers/attributes/heightoffset
    PARSEABLE_VAR(height_offset, float, "HeightOffset")

    // https://blishhud.com/docs/markers/attributes/iconfile
    PARSEABLE_VAR(icon_file, string, "IconFile")

    // https://blishhud.com/docs/markers/attributes/iconsize
    PARSEABLE_VAR(icon_size, float, "IconSize")

    // https://blishhud.com/docs/markers/attributes/info
    PARSEABLE_VAR(info, string, "Info")
    PARSEABLE_VAR(info_range, float, "InfoRange")

    // https://blishhud.com/docs/markers/attributes/invertbehavior
    PARSEABLE_VAR(invert_behavior, bool, "InvertBehavior")

    // https://blishhud.com/docs/markers/attributes/mapdisplaysize
    PARSEABLE_VAR(map_display_size, float, "MapDisplaySize")

    // https://blishhud.com/docs/markers/attributes/mapid
    PARSEABLE_VAR(map_id, int, "MapId")

    // https://blishhud.com/docs/markers/attributes/maptype
    PARSEABLE_VAR(map_type_filter, MapTypeFilter, "MapType")

    // https://blishhud.com/docs/markers/attributes/mount
    PARSEABLE_VAR(mount_filter, MountFilter, "Mount")

    // https://blishhud.com/docs/markers/attributes/position
    PARSEABLE_VAR(position, Position, "Position")
    PARSEABLE_SUBVAR(position, x, Position, float, "XPos", "PositionX")
    PARSEABLE_SUBVAR(position, y, Position, float, "YPos", "PositionY")
    PARSEABLE_SUBVAR(position, z, Position, float, "ZPos", "PositionZ")

    // https://blishhud.com/docs/markers/attributes/profession
    PARSEABLE_VAR(profession_filter, ProfessionFilter, "Profession")

    // https://blishhud.com/docs/markers/attributes/race
    PARSEABLE_VAR(race_filter, RaceFilter, "Race")

    // https://blishhud.com/docs/markers/attributes/resetlength
    PARSEABLE_VAR(reset_length, float, "ResetLength", "ResetLenght")

    // https://blishhud.com/docs/markers/attributes/rotate
    PARSEABLE_VAR(rotation, EulerAngle, "Rotate")
    PARSEABLE_SUBVAR(rotation, x, EulerAngle, float, "RotateX")
    PARSEABLE_SUBVAR(rotation, y, EulerAngle, float, "RotateY")
    PARSEABLE_SUBVAR(rotation, z, EulerAngle, float, "RotateZ")


    // https://blishhud.com/docs/markers/attributes/scaleonmapwithzoom
    PARSEABLE_VAR(scale_on_map_with_zoom, bool, "ScaleOnMapWithZoom")

    // https://blishhud.com/docs/markers/attributes/schedule
    // Not including parsing support for now due to complexity and lack of practical uses
    PARSEABLE_VAR(schedule, string, "Schedule")
    PARSEABLE_VAR(schedule_duration, int, "ScheduleDuration")

    // https://blishhud.com/docs/markers/attributes/showhide
    PARSEABLE_VAR(show, string, "Show")
    PARSEABLE_VAR(hide, string, "Hide")

    // https://blishhud.com/docs/markers/attributes/size
    PARSEABLE_VAR(min_size, float, "MinSize", "nimSize")
    PARSEABLE_VAR(max_size, float, "MaxSize")

    // https://blishhud.com/docs/markers/attributes/specialization
    PARSEABLE_VAR(specialization_filter, SpecializationFilter, "Specialization")

    // https://blishhud.com/docs/markers/attributes/tip
    // INFO: This seems to only be partially defined for blishud between how it
    // works with markers vs categories.
    PARSEABLE_VAR(tip_name, string, "TipName")
    PARSEABLE_VAR(tip_description, string, "TipDescription")

    // https://blishhud.com/docs/markers/attributes/toggle
    PARSEABLE_VAR(toggle, string, "Toggle", "ToggleCategory")

    // https://blishhud.com/docs/markers/attributes/triggerrange
    PARSEABLE_VAR(trigger_range, float, "TriggerRange")

    // https://blishhud.com/docs/markers/attributes/type
    PARSEABLE_VAR(category, string, "Type")

    // https://blishhud.com/docs/markers/attributes/visibility
    PARSEABLE_VAR(visible_on_minimap, bool, "MinimapVisibility", "BHMinimapVisibility")
    PARSEABLE_VAR(visible_on_map, bool, "MapVisibility", "BHMapVisibility")
    PARSEABLE_VAR(visible_ingame, bool, "IngameVisibility", "BHIngameVisibility")

    // Not supported by blishud's docs
    PARSEABLE_VAR(reset_offset, float, "ResetOffset")
    PARSEABLE_VAR(has_countdown, bool, "HasCountdown")

    virtual string classname();
};

#undef PARSEABLE_VAR
#undef PARSEABLE_SUBVAR
