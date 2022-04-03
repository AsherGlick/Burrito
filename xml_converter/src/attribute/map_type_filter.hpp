#pragma once

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "filter.hpp"

using namespace std;

#define FILTER_ITEM(...) CLASS_FILTER_ITEM(MapTypeFilter, __VA_ARGS__ )


class MapTypeFilter: public Filter {
 public:
    // An unknown map type. Used as fallback.
    FILTER_ITEM(unknown_map, "unknown")

    // Redirect map type, e.g. when logging in while in a PvP match.
    FILTER_ITEM(redirect_map, "redirect")

    // Character create map type.
    FILTER_ITEM(character_create_map, "charactercreate")

    // PvP map type.
    FILTER_ITEM(pvp_map, "pvp")

    // GvG map type. Unused.
    FILTER_ITEM(gvg_map, "gvg")

    // Instance map type, e.g. dungeons and story content.
    FILTER_ITEM(instance_map, "instance")

    // Public map type, e.g. open world.
    FILTER_ITEM(public_map, "public")

    // Tournament map type. Probably unused.
    FILTER_ITEM(tournament_map, "tournament")

    // Tutorial map type.
    FILTER_ITEM(tutorial_map, "tutorial")

    // User tournament map type. Probably unused.
    FILTER_ITEM(user_tournament_map, "usertournament")

    // Eternal Battlegrounds (WvW) map type.
    FILTER_ITEM(center_map, "center")

    // Eternal Battlegrounds (WvW) map type.
    FILTER_ITEM(eternal_battlegrounds_map, "eternalbattlegrounds")

    // Blue Borderlands (WvW) map type.
    FILTER_ITEM(bluehome_map, "bluehome")

    // Blue Borderlands (WvW) map type.
    FILTER_ITEM(blue_borderlands_map, "blueborderlands")

    // Green Borderlands (WvW) map type.
    FILTER_ITEM(green_home_map, "greenhome")

    // Green Borderlands (WvW) map type.
    FILTER_ITEM(green_borderlands_map, "greenborderlands")

    // Red Borderlands (WvW) map type.
    FILTER_ITEM(red_home_map, "redhome")

    // Red Borderlands (WvW) map type.
    FILTER_ITEM(red_borderlands_map, "redborderlands")

    // Fortune's Vale. Unused.
    FILTER_ITEM(fortunes_vale_map, "fortunesvale")

    // Obsidian Sanctum (WvW) map type.
    FILTER_ITEM(jump_puzzle_map, "jumppuzzle")

    // Obsidian Sanctum (WvW) map type.
    FILTER_ITEM(obsidian_sanctum_map, "obsidiansanctum")

    // Edge of the Mists (WvW) map type.
    FILTER_ITEM(edge_of_the_mists_map, "edgeofthemists")

    // Mini public map type, e.g. Dry Top, the Silverwastes and Mistlock Sanctuary.
    FILTER_ITEM(public_mini_map, "publicmini")

    // WvW lounge map type, e.g. Armistice Bastion.
    FILTER_ITEM(wvw_lounge_map, "wvwlounge")

    virtual string classname() { return "MapTypeFilter"; }
};

#undef FILTER_ITEM

MapTypeFilter parse_MapTypeFilter(rapidxml::xml_attribute<>* input, vector<string> *errors);
