#include "specialization_filter.hpp"

#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"

SpecializationFilter parse_specialization_filter(rapidxml::xml_attribute<>* input, vector<XMLError*> *errors){
    SpecializationFilter specialization_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
   
    for (string flag_value : flag_values) {
		if (flag_value == "48") {
			specialization_filter.elementalist_tempest = true; 
		}
		else if (flag_value == "Tempest") {
			specialization_filter.elementalist_tempest = true; 
		}
		else if (flag_value == "43") {
			specialization_filter.engineer_scrapper = true; 
		}
		else if (flag_value == "Scrapper") {
			specialization_filter.engineer_scrapper = true; 
		}
		else if (flag_value == "27") {
			specialization_filter.guardian_dragonhunter = true; 
		}
		else if (flag_value == "Dragonhunter") {
			specialization_filter.guardian_dragonhunter = true; 
		}
		else if (flag_value == "40") {
			specialization_filter.mesmer_chronomancer = true; 
		}
		else if (flag_value == "Chronomancer") {
			specialization_filter.mesmer_chronomancer = true; 
		}
		else if (flag_value == "34") {
			specialization_filter.necromancer_reaper = true; 
		}
		else if (flag_value == "Reaper") {
			specialization_filter.necromancer_reaper = true; 
		}
		else if (flag_value == "5") {
			specialization_filter.ranger_druid = true; 
		}
		else if (flag_value == "Druid") {
			specialization_filter.ranger_druid = true; 
		}
		else if (flag_value == "52") {
			specialization_filter.revenant_herald = true; 
		}
		else if (flag_value == "Herald") {
			specialization_filter.revenant_herald = true; 
		}
		else if (flag_value == "7") {
			specialization_filter.thief_daredevil = true; 
		}
		else if (flag_value == "Daredevil") {
			specialization_filter.thief_daredevil = true; 
		}
		else if (flag_value == "18") {
			specialization_filter.warrior_berserker = true; 
		}
		else if (flag_value == "Berserker") {
			specialization_filter.warrior_berserker = true; 
		}
		else if (flag_value == "56") {
			specialization_filter.elementalist_weaver = true; 
		}
		else if (flag_value == "Weaver") {
			specialization_filter.elementalist_weaver = true; 
		}
		else if (flag_value == "57") {
			specialization_filter.engineer_holosmith = true; 
		}
		else if (flag_value == "Holosmith") {
			specialization_filter.engineer_holosmith = true; 
		}
		else if (flag_value == "62") {
			specialization_filter.guardian_firebrand = true; 
		}
		else if (flag_value == "Firebrand") {
			specialization_filter.guardian_firebrand = true; 
		}
		else if (flag_value == "59") {
			specialization_filter.mesmer_mirage = true; 
		}
		else if (flag_value == "Mirage") {
			specialization_filter.mesmer_mirage = true; 
		}
		else if (flag_value == "60") {
			specialization_filter.necromancer_scourge = true; 
		}
		else if (flag_value == "Scourge") {
			specialization_filter.necromancer_scourge = true; 
		}
		else if (flag_value == "55") {
			specialization_filter.ranger_soulbeast = true; 
		}
		else if (flag_value == "Soulbeast") {
			specialization_filter.ranger_soulbeast = true; 
		}
		else if (flag_value == "63") {
			specialization_filter.revenant_renegade = true; 
		}
		else if (flag_value == "Renegade") {
			specialization_filter.revenant_renegade = true; 
		}
		else if (flag_value == "58") {
			specialization_filter.thief_deadeye = true; 
		}
		else if (flag_value == "Deadeye") {
			specialization_filter.thief_deadeye = true; 
		}
		else if (flag_value == "61") {
			specialization_filter.warrior_spellbreaker = true; 
		}
		else if (flag_value == "Spellbreaker") {
			specialization_filter.warrior_spellbreaker = true; 
		}
		else if (flag_value == "Catalyst") {
			specialization_filter.elementalist_catalyst = true; 
		}
		else if (flag_value == "Mechanist") {
			specialization_filter.engineer_mechanist = true; 
		}
		else if (flag_value == "Willbender") {
			specialization_filter.guardian_willbender = true; 
		}
		else if (flag_value == "Virtuoso") {
			specialization_filter.mesmer_virtuoso = true; 
		}
		else if (flag_value == "Harbinger") {
			specialization_filter.necromancer_harbinger = true; 
		}
		else if (flag_value == "Untamed") {
			specialization_filter.ranger_untamed = true; 
		}
		else if (flag_value == "Vindicator") {
			specialization_filter.revenant_vindicator = true; 
		}
		else if (flag_value == "Specter") {
			specialization_filter.thief_specter = true; 
		}
		else if (flag_value == "Bladesworn") {
			specialization_filter.warrior_bladesworn = true; 
		}
		else if (flag_value == "41") {
			specialization_filter.elementalist_air = true; 
		}
		else if (flag_value == "37") {
			specialization_filter.elementalist_arcane = true; 
		}
		else if (flag_value == "26") {
			specialization_filter.elementalist_earth = true; 
		}
		else if (flag_value == "31") {
			specialization_filter.elementalist_fire = true; 
		}
		else if (flag_value == "17") {
			specialization_filter.elementalist_water = true; 
		}
		else if (flag_value == "29") {
			specialization_filter.engineer_alchemy = true; 
		}
		else if (flag_value == "6") {
			specialization_filter.engineer_explosives = true; 
		}
		else if (flag_value == "38") {
			specialization_filter.engineer_firearms = true; 
		}
		else if (flag_value == "47") {
			specialization_filter.engineer_inventions = true; 
		}
		else if (flag_value == "21") {
			specialization_filter.engineer_tools = true; 
		}
		else if (flag_value == "49") {
			specialization_filter.guardian_honor = true; 
		}
		else if (flag_value == "16") {
			specialization_filter.guardian_radiance = true; 
		}
		else if (flag_value == "13") {
			specialization_filter.guardian_valor = true; 
		}
		else if (flag_value == "46") {
			specialization_filter.guardian_virtues = true; 
		}
		else if (flag_value == "42") {
			specialization_filter.guardian_zeal = true; 
		}
		else if (flag_value == "45") {
			specialization_filter.mesmer_chaos = true; 
		}
		else if (flag_value == "10") {
			specialization_filter.mesmer_domination = true; 
		}
		else if (flag_value == "1") {
			specialization_filter.mesmer_dueling = true; 
		}
		else if (flag_value == "24") {
			specialization_filter.mesmer_illusions = true; 
		}
		else if (flag_value == "23") {
			specialization_filter.mesmer_inspiration = true; 
		}
		else if (flag_value == "19") {
			specialization_filter.necromancer_blood_magic = true; 
		}
		else if (flag_value == "39") {
			specialization_filter.necromancer_curses = true; 
		}
		else if (flag_value == "2") {
			specialization_filter.necromancer_death_magic = true; 
		}
		else if (flag_value == "50") {
			specialization_filter.necromancer_soul_reaping = true; 
		}
		else if (flag_value == "53") {
			specialization_filter.necromancer_spite = true; 
		}
		else if (flag_value == "32") {
			specialization_filter.ranger_beastmastery = true; 
		}
		else if (flag_value == "8") {
			specialization_filter.ranger_marksmanship = true; 
		}
		else if (flag_value == "25") {
			specialization_filter.ranger_nature_magic = true; 
		}
		else if (flag_value == "30") {
			specialization_filter.ranger_skirmishing = true; 
		}
		else if (flag_value == "33") {
			specialization_filter.ranger_wilderness_survival = true; 
		}
		else if (flag_value == "14") {
			specialization_filter.revenant_corruption = true; 
		}
		else if (flag_value == "15") {
			specialization_filter.revenant_devastation = true; 
		}
		else if (flag_value == "3") {
			specialization_filter.revenant_invocation = true; 
		}
		else if (flag_value == "9") {
			specialization_filter.revenant_retribution = true; 
		}
		else if (flag_value == "12") {
			specialization_filter.revenant_salvation = true; 
		}
		else if (flag_value == "54") {
			specialization_filter.thief_acrobatics = true; 
		}
		else if (flag_value == "35") {
			specialization_filter.thief_critical_strikes = true; 
		}
		else if (flag_value == "28") {
			specialization_filter.thief_deadly_arts = true; 
		}
		else if (flag_value == "20") {
			specialization_filter.thief_shadow_arts = true; 
		}
		else if (flag_value == "44") {
			specialization_filter.thief_trickery = true; 
		}
		else if (flag_value == "36") {
			specialization_filter.warrior_arms = true; 
		}
		else if (flag_value == "22") {
			specialization_filter.warrior_defense = true; 
		}
		else if (flag_value == "51") {
			specialization_filter.warrior_discipline = true; 
		}
		else if (flag_value == "4") {
			specialization_filter.warrior_strength = true; 
		}
		else if (flag_value == "11") {
			specialization_filter.warrior_tactics = true; 
		}
		else {errors->push_back(new XMLAttributeValueError("Found a value that was not in the class", input));
        	continue;
        }
    }
    return specialization_filter;
	
}