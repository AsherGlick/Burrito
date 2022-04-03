#pragma once

#include "filter.hpp"

#define FILTER_ITEM(...) CLASS_FILTER_ITEM(SpecializationFilter, __VA_ARGS__ )

class SpecializationFilter: public Filter {
	// Heart of Thorns Spec
	FILTER_ITEM(ranger_druid, "5")
	FILTER_ITEM(thief_daredevil, "7")
	FILTER_ITEM(warrior_berserker, "18")
	FILTER_ITEM(guardian_dragonhunter, "27")
	FILTER_ITEM(necromancer_reaper, "34")
	FILTER_ITEM(mesmer_chronomancer, "40")
	FILTER_ITEM(engineer_scrapper, "43")
	FILTER_ITEM(elementalist_tempest, "48")
	FILTER_ITEM(revenant_herald, "52")

	// Path of Fire Spec
	FILTER_ITEM(ranger_soulbeast, "55")
	FILTER_ITEM(elementalist_weaver, "56")
	FILTER_ITEM(engineer_holosmith, "57")
	FILTER_ITEM(thief_deadeye, "58")
	FILTER_ITEM(mesmer_mirage, "59")
	FILTER_ITEM(necromancer_scourge, "60")
	FILTER_ITEM(warrior_spellbreaker, "61")
	FILTER_ITEM(guardian_firebrand, "62")
	FILTER_ITEM(revenant_renegade, "63")

	// TODO: End of Dragons Spec

	// Core Spec
	FILTER_ITEM(mesmer_dueling, "1")
	FILTER_ITEM(necromancer_death_magic, "2")
	FILTER_ITEM(revenant_invocation, "3")
	FILTER_ITEM(warrior_strength, "4")
	FILTER_ITEM(engineer_explosives, "6")
	FILTER_ITEM(ranger_marksmanship, "8")
	FILTER_ITEM(revenant_retribution, "9")
	FILTER_ITEM(mesmer_domination, "10")
	FILTER_ITEM(warrior_tactics, "11")
	FILTER_ITEM(revenant_salvation, "12")
	FILTER_ITEM(guardian_valor, "13")
	FILTER_ITEM(revenant_corruption, "14")
	FILTER_ITEM(revenant_devastation, "15")
	FILTER_ITEM(guardian_radiance, "16")
	FILTER_ITEM(elementalist_water, "17")
	FILTER_ITEM(necromancer_blood_magic, "19")
	FILTER_ITEM(thief_shadow_arts, "20")
	FILTER_ITEM(engineer_tools, "21")
	FILTER_ITEM(warrior_defense, "22")
	FILTER_ITEM(mesmer_inspiration, "23")
	FILTER_ITEM(mesmer_illusions, "24")
	FILTER_ITEM(ranger_nature_magic, "25")
	FILTER_ITEM(elementalist_earth, "26")
	FILTER_ITEM(thief_deadly_arts, "28")
	FILTER_ITEM(engineer_alchemy, "29")
	FILTER_ITEM(ranger_skirmishing, "30")
	FILTER_ITEM(elementalist_fire, "31")
	FILTER_ITEM(ranger_beastmastery, "32")
	FILTER_ITEM(ranger_wilderness_survival, "33")
	FILTER_ITEM(thief_critical_strikes, "35")
	FILTER_ITEM(warrior_arms, "36")
	FILTER_ITEM(elementalist_arcane, "37")
	FILTER_ITEM(engineer_firearms, "38")
	FILTER_ITEM(necromancer_curses, "39")
	FILTER_ITEM(elementalist_air, "41")
	FILTER_ITEM(guardian_zeal, "42")
	FILTER_ITEM(thief_trickery, "44")
	FILTER_ITEM(mesmer_chaos, "45")
	FILTER_ITEM(guardian_virtues, "46")
	FILTER_ITEM(engineer_inventions, "47")
	FILTER_ITEM(guardian_honor, "49")
	FILTER_ITEM(necromancer_soul_reaping, "50")
	FILTER_ITEM(warrior_discipline, "51")
	FILTER_ITEM(necromancer_spite, "53")
	FILTER_ITEM(thief_acrobatics, "54")

    virtual string classname() { return "SpecializationFilter"; }

};

#undef FILTER_ITEM

SpecializationFilter parse_SpecializationFilter(rapidxml::xml_attribute<>* input, vector<string> *errors);