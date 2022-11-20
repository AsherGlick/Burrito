#include "specialization_filter_gen.hpp"

#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"

using namespace std;

SpecializationFilter parse_specialization_filter(rapidxml::xml_attribute<>* input, vector<XMLError*>* errors) {
    SpecializationFilter specialization_filter;
    vector<string> flag_values;
    flag_values = split(get_attribute_value(input), ",");
    specialization_filter.elementalist_tempest = false;
    specialization_filter.engineer_scrapper = false;
    specialization_filter.guardian_dragonhunter = false;
    specialization_filter.mesmer_chronomancer = false;
    specialization_filter.necromancer_reaper = false;
    specialization_filter.ranger_druid = false;
    specialization_filter.revenant_herald = false;
    specialization_filter.thief_daredevil = false;
    specialization_filter.warrior_berserker = false;
    specialization_filter.elementalist_weaver = false;
    specialization_filter.engineer_holosmith = false;
    specialization_filter.guardian_firebrand = false;
    specialization_filter.mesmer_mirage = false;
    specialization_filter.necromancer_scourge = false;
    specialization_filter.ranger_soulbeast = false;
    specialization_filter.revenant_renegade = false;
    specialization_filter.thief_deadeye = false;
    specialization_filter.warrior_spellbreaker = false;
    specialization_filter.elementalist_catalyst = false;
    specialization_filter.engineer_mechanist = false;
    specialization_filter.guardian_willbender = false;
    specialization_filter.mesmer_virtuoso = false;
    specialization_filter.necromancer_harbinger = false;
    specialization_filter.ranger_untamed = false;
    specialization_filter.revenant_vindicator = false;
    specialization_filter.thief_specter = false;
    specialization_filter.warrior_bladesworn = false;
    specialization_filter.elementalist_air = false;
    specialization_filter.elementalist_arcane = false;
    specialization_filter.elementalist_earth = false;
    specialization_filter.elementalist_fire = false;
    specialization_filter.elementalist_water = false;
    specialization_filter.engineer_alchemy = false;
    specialization_filter.engineer_explosives = false;
    specialization_filter.engineer_firearms = false;
    specialization_filter.engineer_inventions = false;
    specialization_filter.engineer_tools = false;
    specialization_filter.guardian_honor = false;
    specialization_filter.guardian_radiance = false;
    specialization_filter.guardian_valor = false;
    specialization_filter.guardian_virtues = false;
    specialization_filter.guardian_zeal = false;
    specialization_filter.mesmer_chaos = false;
    specialization_filter.mesmer_domination = false;
    specialization_filter.mesmer_dueling = false;
    specialization_filter.mesmer_illusions = false;
    specialization_filter.mesmer_inspiration = false;
    specialization_filter.necromancer_blood_magic = false;
    specialization_filter.necromancer_curses = false;
    specialization_filter.necromancer_death_magic = false;
    specialization_filter.necromancer_soul_reaping = false;
    specialization_filter.necromancer_spite = false;
    specialization_filter.ranger_beastmastery = false;
    specialization_filter.ranger_marksmanship = false;
    specialization_filter.ranger_nature_magic = false;
    specialization_filter.ranger_skirmishing = false;
    specialization_filter.ranger_wilderness_survival = false;
    specialization_filter.revenant_corruption = false;
    specialization_filter.revenant_devastation = false;
    specialization_filter.revenant_invocation = false;
    specialization_filter.revenant_retribution = false;
    specialization_filter.revenant_salvation = false;
    specialization_filter.thief_acrobatics = false;
    specialization_filter.thief_critical_strikes = false;
    specialization_filter.thief_deadly_arts = false;
    specialization_filter.thief_shadow_arts = false;
    specialization_filter.thief_trickery = false;
    specialization_filter.warrior_arms = false;
    specialization_filter.warrior_defense = false;
    specialization_filter.warrior_discipline = false;
    specialization_filter.warrior_strength = false;
    specialization_filter.warrior_tactics = false;

    for (string flag_value : flag_values) {
        string normalized_flag_value = normalize(flag_value);
        if (normalized_flag_value == "48") {
            specialization_filter.elementalist_tempest = true;
        }
        else if (normalized_flag_value == "tempest") {
            specialization_filter.elementalist_tempest = true;
        }
        else if (normalized_flag_value == "43") {
            specialization_filter.engineer_scrapper = true;
        }
        else if (normalized_flag_value == "scrapper") {
            specialization_filter.engineer_scrapper = true;
        }
        else if (normalized_flag_value == "27") {
            specialization_filter.guardian_dragonhunter = true;
        }
        else if (normalized_flag_value == "dragonhunter") {
            specialization_filter.guardian_dragonhunter = true;
        }
        else if (normalized_flag_value == "40") {
            specialization_filter.mesmer_chronomancer = true;
        }
        else if (normalized_flag_value == "chronomancer") {
            specialization_filter.mesmer_chronomancer = true;
        }
        else if (normalized_flag_value == "34") {
            specialization_filter.necromancer_reaper = true;
        }
        else if (normalized_flag_value == "reaper") {
            specialization_filter.necromancer_reaper = true;
        }
        else if (normalized_flag_value == "5") {
            specialization_filter.ranger_druid = true;
        }
        else if (normalized_flag_value == "druid") {
            specialization_filter.ranger_druid = true;
        }
        else if (normalized_flag_value == "52") {
            specialization_filter.revenant_herald = true;
        }
        else if (normalized_flag_value == "herald") {
            specialization_filter.revenant_herald = true;
        }
        else if (normalized_flag_value == "7") {
            specialization_filter.thief_daredevil = true;
        }
        else if (normalized_flag_value == "daredevil") {
            specialization_filter.thief_daredevil = true;
        }
        else if (normalized_flag_value == "18") {
            specialization_filter.warrior_berserker = true;
        }
        else if (normalized_flag_value == "berserker") {
            specialization_filter.warrior_berserker = true;
        }
        else if (normalized_flag_value == "56") {
            specialization_filter.elementalist_weaver = true;
        }
        else if (normalized_flag_value == "weaver") {
            specialization_filter.elementalist_weaver = true;
        }
        else if (normalized_flag_value == "57") {
            specialization_filter.engineer_holosmith = true;
        }
        else if (normalized_flag_value == "holosmith") {
            specialization_filter.engineer_holosmith = true;
        }
        else if (normalized_flag_value == "62") {
            specialization_filter.guardian_firebrand = true;
        }
        else if (normalized_flag_value == "firebrand") {
            specialization_filter.guardian_firebrand = true;
        }
        else if (normalized_flag_value == "59") {
            specialization_filter.mesmer_mirage = true;
        }
        else if (normalized_flag_value == "mirage") {
            specialization_filter.mesmer_mirage = true;
        }
        else if (normalized_flag_value == "60") {
            specialization_filter.necromancer_scourge = true;
        }
        else if (normalized_flag_value == "scourge") {
            specialization_filter.necromancer_scourge = true;
        }
        else if (normalized_flag_value == "55") {
            specialization_filter.ranger_soulbeast = true;
        }
        else if (normalized_flag_value == "soulbeast") {
            specialization_filter.ranger_soulbeast = true;
        }
        else if (normalized_flag_value == "63") {
            specialization_filter.revenant_renegade = true;
        }
        else if (normalized_flag_value == "renegade") {
            specialization_filter.revenant_renegade = true;
        }
        else if (normalized_flag_value == "58") {
            specialization_filter.thief_deadeye = true;
        }
        else if (normalized_flag_value == "deadeye") {
            specialization_filter.thief_deadeye = true;
        }
        else if (normalized_flag_value == "61") {
            specialization_filter.warrior_spellbreaker = true;
        }
        else if (normalized_flag_value == "spellbreaker") {
            specialization_filter.warrior_spellbreaker = true;
        }
        else if (normalized_flag_value == "67") {
            specialization_filter.elementalist_catalyst = true;
        }
        else if (normalized_flag_value == "catalyst") {
            specialization_filter.elementalist_catalyst = true;
        }
        else if (normalized_flag_value == "70") {
            specialization_filter.engineer_mechanist = true;
        }
        else if (normalized_flag_value == "mechanist") {
            specialization_filter.engineer_mechanist = true;
        }
        else if (normalized_flag_value == "65") {
            specialization_filter.guardian_willbender = true;
        }
        else if (normalized_flag_value == "willbender") {
            specialization_filter.guardian_willbender = true;
        }
        else if (normalized_flag_value == "66") {
            specialization_filter.mesmer_virtuoso = true;
        }
        else if (normalized_flag_value == "virtuoso") {
            specialization_filter.mesmer_virtuoso = true;
        }
        else if (normalized_flag_value == "64") {
            specialization_filter.necromancer_harbinger = true;
        }
        else if (normalized_flag_value == "harbinger") {
            specialization_filter.necromancer_harbinger = true;
        }
        else if (normalized_flag_value == "72") {
            specialization_filter.ranger_untamed = true;
        }
        else if (normalized_flag_value == "untamed") {
            specialization_filter.ranger_untamed = true;
        }
        else if (normalized_flag_value == "69") {
            specialization_filter.revenant_vindicator = true;
        }
        else if (normalized_flag_value == "vindicator") {
            specialization_filter.revenant_vindicator = true;
        }
        else if (normalized_flag_value == "71") {
            specialization_filter.thief_specter = true;
        }
        else if (normalized_flag_value == "specter") {
            specialization_filter.thief_specter = true;
        }
        else if (normalized_flag_value == "68") {
            specialization_filter.warrior_bladesworn = true;
        }
        else if (normalized_flag_value == "bladesworn") {
            specialization_filter.warrior_bladesworn = true;
        }
        else if (normalized_flag_value == "41") {
            specialization_filter.elementalist_air = true;
        }
        else if (normalized_flag_value == "37") {
            specialization_filter.elementalist_arcane = true;
        }
        else if (normalized_flag_value == "26") {
            specialization_filter.elementalist_earth = true;
        }
        else if (normalized_flag_value == "31") {
            specialization_filter.elementalist_fire = true;
        }
        else if (normalized_flag_value == "17") {
            specialization_filter.elementalist_water = true;
        }
        else if (normalized_flag_value == "29") {
            specialization_filter.engineer_alchemy = true;
        }
        else if (normalized_flag_value == "6") {
            specialization_filter.engineer_explosives = true;
        }
        else if (normalized_flag_value == "38") {
            specialization_filter.engineer_firearms = true;
        }
        else if (normalized_flag_value == "47") {
            specialization_filter.engineer_inventions = true;
        }
        else if (normalized_flag_value == "21") {
            specialization_filter.engineer_tools = true;
        }
        else if (normalized_flag_value == "49") {
            specialization_filter.guardian_honor = true;
        }
        else if (normalized_flag_value == "16") {
            specialization_filter.guardian_radiance = true;
        }
        else if (normalized_flag_value == "13") {
            specialization_filter.guardian_valor = true;
        }
        else if (normalized_flag_value == "46") {
            specialization_filter.guardian_virtues = true;
        }
        else if (normalized_flag_value == "42") {
            specialization_filter.guardian_zeal = true;
        }
        else if (normalized_flag_value == "45") {
            specialization_filter.mesmer_chaos = true;
        }
        else if (normalized_flag_value == "10") {
            specialization_filter.mesmer_domination = true;
        }
        else if (normalized_flag_value == "1") {
            specialization_filter.mesmer_dueling = true;
        }
        else if (normalized_flag_value == "24") {
            specialization_filter.mesmer_illusions = true;
        }
        else if (normalized_flag_value == "23") {
            specialization_filter.mesmer_inspiration = true;
        }
        else if (normalized_flag_value == "19") {
            specialization_filter.necromancer_blood_magic = true;
        }
        else if (normalized_flag_value == "39") {
            specialization_filter.necromancer_curses = true;
        }
        else if (normalized_flag_value == "2") {
            specialization_filter.necromancer_death_magic = true;
        }
        else if (normalized_flag_value == "50") {
            specialization_filter.necromancer_soul_reaping = true;
        }
        else if (normalized_flag_value == "53") {
            specialization_filter.necromancer_spite = true;
        }
        else if (normalized_flag_value == "32") {
            specialization_filter.ranger_beastmastery = true;
        }
        else if (normalized_flag_value == "8") {
            specialization_filter.ranger_marksmanship = true;
        }
        else if (normalized_flag_value == "25") {
            specialization_filter.ranger_nature_magic = true;
        }
        else if (normalized_flag_value == "30") {
            specialization_filter.ranger_skirmishing = true;
        }
        else if (normalized_flag_value == "33") {
            specialization_filter.ranger_wilderness_survival = true;
        }
        else if (normalized_flag_value == "14") {
            specialization_filter.revenant_corruption = true;
        }
        else if (normalized_flag_value == "15") {
            specialization_filter.revenant_devastation = true;
        }
        else if (normalized_flag_value == "3") {
            specialization_filter.revenant_invocation = true;
        }
        else if (normalized_flag_value == "9") {
            specialization_filter.revenant_retribution = true;
        }
        else if (normalized_flag_value == "12") {
            specialization_filter.revenant_salvation = true;
        }
        else if (normalized_flag_value == "54") {
            specialization_filter.thief_acrobatics = true;
        }
        else if (normalized_flag_value == "35") {
            specialization_filter.thief_critical_strikes = true;
        }
        else if (normalized_flag_value == "28") {
            specialization_filter.thief_deadly_arts = true;
        }
        else if (normalized_flag_value == "20") {
            specialization_filter.thief_shadow_arts = true;
        }
        else if (normalized_flag_value == "44") {
            specialization_filter.thief_trickery = true;
        }
        else if (normalized_flag_value == "36") {
            specialization_filter.warrior_arms = true;
        }
        else if (normalized_flag_value == "22") {
            specialization_filter.warrior_defense = true;
        }
        else if (normalized_flag_value == "51") {
            specialization_filter.warrior_discipline = true;
        }
        else if (normalized_flag_value == "4") {
            specialization_filter.warrior_strength = true;
        }
        else if (normalized_flag_value == "11") {
            specialization_filter.warrior_tactics = true;
        }
        else {
            errors->push_back(new XMLAttributeValueError("Invalid Filter for SpecializationFilter. Found " + flag_value, input));
            continue;
        }
    }
    return specialization_filter;
}

string stringify_specialization_filter(SpecializationFilter attribute_value) {
    string output = "";
    if (attribute_value.elementalist_tempest == true) {
        output = output + "48";
    }
    if (attribute_value.engineer_scrapper == true) {
        output = output + "43";
    }
    if (attribute_value.guardian_dragonhunter == true) {
        output = output + "27";
    }
    if (attribute_value.mesmer_chronomancer == true) {
        output = output + "40";
    }
    if (attribute_value.necromancer_reaper == true) {
        output = output + "34";
    }
    if (attribute_value.ranger_druid == true) {
        output = output + "5";
    }
    if (attribute_value.revenant_herald == true) {
        output = output + "52";
    }
    if (attribute_value.thief_daredevil == true) {
        output = output + "7";
    }
    if (attribute_value.warrior_berserker == true) {
        output = output + "18";
    }
    if (attribute_value.elementalist_weaver == true) {
        output = output + "56";
    }
    if (attribute_value.engineer_holosmith == true) {
        output = output + "57";
    }
    if (attribute_value.guardian_firebrand == true) {
        output = output + "62";
    }
    if (attribute_value.mesmer_mirage == true) {
        output = output + "59";
    }
    if (attribute_value.necromancer_scourge == true) {
        output = output + "60";
    }
    if (attribute_value.ranger_soulbeast == true) {
        output = output + "55";
    }
    if (attribute_value.revenant_renegade == true) {
        output = output + "63";
    }
    if (attribute_value.thief_deadeye == true) {
        output = output + "58";
    }
    if (attribute_value.warrior_spellbreaker == true) {
        output = output + "61";
    }
    if (attribute_value.elementalist_catalyst == true) {
        output = output + "67";
    }
    if (attribute_value.engineer_mechanist == true) {
        output = output + "70";
    }
    if (attribute_value.guardian_willbender == true) {
        output = output + "65";
    }
    if (attribute_value.mesmer_virtuoso == true) {
        output = output + "66";
    }
    if (attribute_value.necromancer_harbinger == true) {
        output = output + "64";
    }
    if (attribute_value.ranger_untamed == true) {
        output = output + "72";
    }
    if (attribute_value.revenant_vindicator == true) {
        output = output + "69";
    }
    if (attribute_value.thief_specter == true) {
        output = output + "71";
    }
    if (attribute_value.warrior_bladesworn == true) {
        output = output + "68";
    }
    if (attribute_value.elementalist_air == true) {
        output = output + "41";
    }
    if (attribute_value.elementalist_arcane == true) {
        output = output + "37";
    }
    if (attribute_value.elementalist_earth == true) {
        output = output + "26";
    }
    if (attribute_value.elementalist_fire == true) {
        output = output + "31";
    }
    if (attribute_value.elementalist_water == true) {
        output = output + "17";
    }
    if (attribute_value.engineer_alchemy == true) {
        output = output + "29";
    }
    if (attribute_value.engineer_explosives == true) {
        output = output + "6";
    }
    if (attribute_value.engineer_firearms == true) {
        output = output + "38";
    }
    if (attribute_value.engineer_inventions == true) {
        output = output + "47";
    }
    if (attribute_value.engineer_tools == true) {
        output = output + "21";
    }
    if (attribute_value.guardian_honor == true) {
        output = output + "49";
    }
    if (attribute_value.guardian_radiance == true) {
        output = output + "16";
    }
    if (attribute_value.guardian_valor == true) {
        output = output + "13";
    }
    if (attribute_value.guardian_virtues == true) {
        output = output + "46";
    }
    if (attribute_value.guardian_zeal == true) {
        output = output + "42";
    }
    if (attribute_value.mesmer_chaos == true) {
        output = output + "45";
    }
    if (attribute_value.mesmer_domination == true) {
        output = output + "10";
    }
    if (attribute_value.mesmer_dueling == true) {
        output = output + "1";
    }
    if (attribute_value.mesmer_illusions == true) {
        output = output + "24";
    }
    if (attribute_value.mesmer_inspiration == true) {
        output = output + "23";
    }
    if (attribute_value.necromancer_blood_magic == true) {
        output = output + "19";
    }
    if (attribute_value.necromancer_curses == true) {
        output = output + "39";
    }
    if (attribute_value.necromancer_death_magic == true) {
        output = output + "2";
    }
    if (attribute_value.necromancer_soul_reaping == true) {
        output = output + "50";
    }
    if (attribute_value.necromancer_spite == true) {
        output = output + "53";
    }
    if (attribute_value.ranger_beastmastery == true) {
        output = output + "32";
    }
    if (attribute_value.ranger_marksmanship == true) {
        output = output + "8";
    }
    if (attribute_value.ranger_nature_magic == true) {
        output = output + "25";
    }
    if (attribute_value.ranger_skirmishing == true) {
        output = output + "30";
    }
    if (attribute_value.ranger_wilderness_survival == true) {
        output = output + "33";
    }
    if (attribute_value.revenant_corruption == true) {
        output = output + "14";
    }
    if (attribute_value.revenant_devastation == true) {
        output = output + "15";
    }
    if (attribute_value.revenant_invocation == true) {
        output = output + "3";
    }
    if (attribute_value.revenant_retribution == true) {
        output = output + "9";
    }
    if (attribute_value.revenant_salvation == true) {
        output = output + "12";
    }
    if (attribute_value.thief_acrobatics == true) {
        output = output + "54";
    }
    if (attribute_value.thief_critical_strikes == true) {
        output = output + "35";
    }
    if (attribute_value.thief_deadly_arts == true) {
        output = output + "28";
    }
    if (attribute_value.thief_shadow_arts == true) {
        output = output + "20";
    }
    if (attribute_value.thief_trickery == true) {
        output = output + "44";
    }
    if (attribute_value.warrior_arms == true) {
        output = output + "36";
    }
    if (attribute_value.warrior_defense == true) {
        output = output + "22";
    }
    if (attribute_value.warrior_discipline == true) {
        output = output + "51";
    }
    if (attribute_value.warrior_strength == true) {
        output = output + "4";
    }
    if (attribute_value.warrior_tactics == true) {
        output = output + "11";
    }
    return output;
}
