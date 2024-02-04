#include "specialization_filter_gen.hpp"

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

#include "../rapid_helpers.hpp"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../string_helper.hpp"
#include "waypoint.pb.h"

using namespace std;

void xml_attribute_to_specialization_filter(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState*,
    SpecializationFilter* value,
    bool* is_set) {
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
    *value = specialization_filter;
    *is_set = true;
}

string specialization_filter_to_xml_attribute(
    const std::string& attribute_name,
    XMLWriterState*,
    const SpecializationFilter* value) {
    vector<string> flag_values;
    if (value->elementalist_tempest == true) {
        flag_values.push_back("48");
    }
    if (value->engineer_scrapper == true) {
        flag_values.push_back("43");
    }
    if (value->guardian_dragonhunter == true) {
        flag_values.push_back("27");
    }
    if (value->mesmer_chronomancer == true) {
        flag_values.push_back("40");
    }
    if (value->necromancer_reaper == true) {
        flag_values.push_back("34");
    }
    if (value->ranger_druid == true) {
        flag_values.push_back("5");
    }
    if (value->revenant_herald == true) {
        flag_values.push_back("52");
    }
    if (value->thief_daredevil == true) {
        flag_values.push_back("7");
    }
    if (value->warrior_berserker == true) {
        flag_values.push_back("18");
    }
    if (value->elementalist_weaver == true) {
        flag_values.push_back("56");
    }
    if (value->engineer_holosmith == true) {
        flag_values.push_back("57");
    }
    if (value->guardian_firebrand == true) {
        flag_values.push_back("62");
    }
    if (value->mesmer_mirage == true) {
        flag_values.push_back("59");
    }
    if (value->necromancer_scourge == true) {
        flag_values.push_back("60");
    }
    if (value->ranger_soulbeast == true) {
        flag_values.push_back("55");
    }
    if (value->revenant_renegade == true) {
        flag_values.push_back("63");
    }
    if (value->thief_deadeye == true) {
        flag_values.push_back("58");
    }
    if (value->warrior_spellbreaker == true) {
        flag_values.push_back("61");
    }
    if (value->elementalist_catalyst == true) {
        flag_values.push_back("67");
    }
    if (value->engineer_mechanist == true) {
        flag_values.push_back("70");
    }
    if (value->guardian_willbender == true) {
        flag_values.push_back("65");
    }
    if (value->mesmer_virtuoso == true) {
        flag_values.push_back("66");
    }
    if (value->necromancer_harbinger == true) {
        flag_values.push_back("64");
    }
    if (value->ranger_untamed == true) {
        flag_values.push_back("72");
    }
    if (value->revenant_vindicator == true) {
        flag_values.push_back("69");
    }
    if (value->thief_specter == true) {
        flag_values.push_back("71");
    }
    if (value->warrior_bladesworn == true) {
        flag_values.push_back("68");
    }
    if (value->elementalist_air == true) {
        flag_values.push_back("41");
    }
    if (value->elementalist_arcane == true) {
        flag_values.push_back("37");
    }
    if (value->elementalist_earth == true) {
        flag_values.push_back("26");
    }
    if (value->elementalist_fire == true) {
        flag_values.push_back("31");
    }
    if (value->elementalist_water == true) {
        flag_values.push_back("17");
    }
    if (value->engineer_alchemy == true) {
        flag_values.push_back("29");
    }
    if (value->engineer_explosives == true) {
        flag_values.push_back("6");
    }
    if (value->engineer_firearms == true) {
        flag_values.push_back("38");
    }
    if (value->engineer_inventions == true) {
        flag_values.push_back("47");
    }
    if (value->engineer_tools == true) {
        flag_values.push_back("21");
    }
    if (value->guardian_honor == true) {
        flag_values.push_back("49");
    }
    if (value->guardian_radiance == true) {
        flag_values.push_back("16");
    }
    if (value->guardian_valor == true) {
        flag_values.push_back("13");
    }
    if (value->guardian_virtues == true) {
        flag_values.push_back("46");
    }
    if (value->guardian_zeal == true) {
        flag_values.push_back("42");
    }
    if (value->mesmer_chaos == true) {
        flag_values.push_back("45");
    }
    if (value->mesmer_domination == true) {
        flag_values.push_back("10");
    }
    if (value->mesmer_dueling == true) {
        flag_values.push_back("1");
    }
    if (value->mesmer_illusions == true) {
        flag_values.push_back("24");
    }
    if (value->mesmer_inspiration == true) {
        flag_values.push_back("23");
    }
    if (value->necromancer_blood_magic == true) {
        flag_values.push_back("19");
    }
    if (value->necromancer_curses == true) {
        flag_values.push_back("39");
    }
    if (value->necromancer_death_magic == true) {
        flag_values.push_back("2");
    }
    if (value->necromancer_soul_reaping == true) {
        flag_values.push_back("50");
    }
    if (value->necromancer_spite == true) {
        flag_values.push_back("53");
    }
    if (value->ranger_beastmastery == true) {
        flag_values.push_back("32");
    }
    if (value->ranger_marksmanship == true) {
        flag_values.push_back("8");
    }
    if (value->ranger_nature_magic == true) {
        flag_values.push_back("25");
    }
    if (value->ranger_skirmishing == true) {
        flag_values.push_back("30");
    }
    if (value->ranger_wilderness_survival == true) {
        flag_values.push_back("33");
    }
    if (value->revenant_corruption == true) {
        flag_values.push_back("14");
    }
    if (value->revenant_devastation == true) {
        flag_values.push_back("15");
    }
    if (value->revenant_invocation == true) {
        flag_values.push_back("3");
    }
    if (value->revenant_retribution == true) {
        flag_values.push_back("9");
    }
    if (value->revenant_salvation == true) {
        flag_values.push_back("12");
    }
    if (value->thief_acrobatics == true) {
        flag_values.push_back("54");
    }
    if (value->thief_critical_strikes == true) {
        flag_values.push_back("35");
    }
    if (value->thief_deadly_arts == true) {
        flag_values.push_back("28");
    }
    if (value->thief_shadow_arts == true) {
        flag_values.push_back("20");
    }
    if (value->thief_trickery == true) {
        flag_values.push_back("44");
    }
    if (value->warrior_arms == true) {
        flag_values.push_back("36");
    }
    if (value->warrior_defense == true) {
        flag_values.push_back("22");
    }
    if (value->warrior_discipline == true) {
        flag_values.push_back("51");
    }
    if (value->warrior_strength == true) {
        flag_values.push_back("4");
    }
    if (value->warrior_tactics == true) {
        flag_values.push_back("11");
    }
    string output = join(flag_values, ",");
    return " " + attribute_name + "=\"" + output + "\"";
}

void proto_to_specialization_filter(
    waypoint::SpecializationFilter input,
    ProtoReaderState*,
    SpecializationFilter* value,
    bool* is_set) {
    SpecializationFilter specialization_filter;
    specialization_filter.elementalist_tempest = input.elementalist_tempest();
    specialization_filter.engineer_scrapper = input.engineer_scrapper();
    specialization_filter.guardian_dragonhunter = input.guardian_dragonhunter();
    specialization_filter.mesmer_chronomancer = input.mesmer_chronomancer();
    specialization_filter.necromancer_reaper = input.necromancer_reaper();
    specialization_filter.ranger_druid = input.ranger_druid();
    specialization_filter.revenant_herald = input.revenant_herald();
    specialization_filter.thief_daredevil = input.thief_daredevil();
    specialization_filter.warrior_berserker = input.warrior_berserker();
    specialization_filter.elementalist_weaver = input.elementalist_weaver();
    specialization_filter.engineer_holosmith = input.engineer_holosmith();
    specialization_filter.guardian_firebrand = input.guardian_firebrand();
    specialization_filter.mesmer_mirage = input.mesmer_mirage();
    specialization_filter.necromancer_scourge = input.necromancer_scourge();
    specialization_filter.ranger_soulbeast = input.ranger_soulbeast();
    specialization_filter.revenant_renegade = input.revenant_renegade();
    specialization_filter.thief_deadeye = input.thief_deadeye();
    specialization_filter.warrior_spellbreaker = input.warrior_spellbreaker();
    specialization_filter.elementalist_catalyst = input.elementalist_catalyst();
    specialization_filter.engineer_mechanist = input.engineer_mechanist();
    specialization_filter.guardian_willbender = input.guardian_willbender();
    specialization_filter.mesmer_virtuoso = input.mesmer_virtuoso();
    specialization_filter.necromancer_harbinger = input.necromancer_harbinger();
    specialization_filter.ranger_untamed = input.ranger_untamed();
    specialization_filter.revenant_vindicator = input.revenant_vindicator();
    specialization_filter.thief_specter = input.thief_specter();
    specialization_filter.warrior_bladesworn = input.warrior_bladesworn();
    specialization_filter.elementalist_air = input.elementalist_air();
    specialization_filter.elementalist_arcane = input.elementalist_arcane();
    specialization_filter.elementalist_earth = input.elementalist_earth();
    specialization_filter.elementalist_fire = input.elementalist_fire();
    specialization_filter.elementalist_water = input.elementalist_water();
    specialization_filter.engineer_alchemy = input.engineer_alchemy();
    specialization_filter.engineer_explosives = input.engineer_explosives();
    specialization_filter.engineer_firearms = input.engineer_firearms();
    specialization_filter.engineer_inventions = input.engineer_inventions();
    specialization_filter.engineer_tools = input.engineer_tools();
    specialization_filter.guardian_honor = input.guardian_honor();
    specialization_filter.guardian_radiance = input.guardian_radiance();
    specialization_filter.guardian_valor = input.guardian_valor();
    specialization_filter.guardian_virtues = input.guardian_virtues();
    specialization_filter.guardian_zeal = input.guardian_zeal();
    specialization_filter.mesmer_chaos = input.mesmer_chaos();
    specialization_filter.mesmer_domination = input.mesmer_domination();
    specialization_filter.mesmer_dueling = input.mesmer_dueling();
    specialization_filter.mesmer_illusions = input.mesmer_illusions();
    specialization_filter.mesmer_inspiration = input.mesmer_inspiration();
    specialization_filter.necromancer_blood_magic = input.necromancer_blood_magic();
    specialization_filter.necromancer_curses = input.necromancer_curses();
    specialization_filter.necromancer_death_magic = input.necromancer_death_magic();
    specialization_filter.necromancer_soul_reaping = input.necromancer_soul_reaping();
    specialization_filter.necromancer_spite = input.necromancer_spite();
    specialization_filter.ranger_beastmastery = input.ranger_beastmastery();
    specialization_filter.ranger_marksmanship = input.ranger_marksmanship();
    specialization_filter.ranger_nature_magic = input.ranger_nature_magic();
    specialization_filter.ranger_skirmishing = input.ranger_skirmishing();
    specialization_filter.ranger_wilderness_survival = input.ranger_wilderness_survival();
    specialization_filter.revenant_corruption = input.revenant_corruption();
    specialization_filter.revenant_devastation = input.revenant_devastation();
    specialization_filter.revenant_invocation = input.revenant_invocation();
    specialization_filter.revenant_retribution = input.revenant_retribution();
    specialization_filter.revenant_salvation = input.revenant_salvation();
    specialization_filter.thief_acrobatics = input.thief_acrobatics();
    specialization_filter.thief_critical_strikes = input.thief_critical_strikes();
    specialization_filter.thief_deadly_arts = input.thief_deadly_arts();
    specialization_filter.thief_shadow_arts = input.thief_shadow_arts();
    specialization_filter.thief_trickery = input.thief_trickery();
    specialization_filter.warrior_arms = input.warrior_arms();
    specialization_filter.warrior_defense = input.warrior_defense();
    specialization_filter.warrior_discipline = input.warrior_discipline();
    specialization_filter.warrior_strength = input.warrior_strength();
    specialization_filter.warrior_tactics = input.warrior_tactics();
    *value = specialization_filter;
    *is_set = true;
}

void specialization_filter_to_proto(
    SpecializationFilter value,
    ProtoWriterState*,
    std::function<void(waypoint::SpecializationFilter*)> setter) {
    waypoint::SpecializationFilter* proto_specialization_filter = new waypoint::SpecializationFilter();
    bool should_write = false;
    proto_specialization_filter->set_elementalist_tempest(value.elementalist_tempest);
    should_write |= value.elementalist_tempest;
    proto_specialization_filter->set_engineer_scrapper(value.engineer_scrapper);
    should_write |= value.engineer_scrapper;
    proto_specialization_filter->set_guardian_dragonhunter(value.guardian_dragonhunter);
    should_write |= value.guardian_dragonhunter;
    proto_specialization_filter->set_mesmer_chronomancer(value.mesmer_chronomancer);
    should_write |= value.mesmer_chronomancer;
    proto_specialization_filter->set_necromancer_reaper(value.necromancer_reaper);
    should_write |= value.necromancer_reaper;
    proto_specialization_filter->set_ranger_druid(value.ranger_druid);
    should_write |= value.ranger_druid;
    proto_specialization_filter->set_revenant_herald(value.revenant_herald);
    should_write |= value.revenant_herald;
    proto_specialization_filter->set_thief_daredevil(value.thief_daredevil);
    should_write |= value.thief_daredevil;
    proto_specialization_filter->set_warrior_berserker(value.warrior_berserker);
    should_write |= value.warrior_berserker;
    proto_specialization_filter->set_elementalist_weaver(value.elementalist_weaver);
    should_write |= value.elementalist_weaver;
    proto_specialization_filter->set_engineer_holosmith(value.engineer_holosmith);
    should_write |= value.engineer_holosmith;
    proto_specialization_filter->set_guardian_firebrand(value.guardian_firebrand);
    should_write |= value.guardian_firebrand;
    proto_specialization_filter->set_mesmer_mirage(value.mesmer_mirage);
    should_write |= value.mesmer_mirage;
    proto_specialization_filter->set_necromancer_scourge(value.necromancer_scourge);
    should_write |= value.necromancer_scourge;
    proto_specialization_filter->set_ranger_soulbeast(value.ranger_soulbeast);
    should_write |= value.ranger_soulbeast;
    proto_specialization_filter->set_revenant_renegade(value.revenant_renegade);
    should_write |= value.revenant_renegade;
    proto_specialization_filter->set_thief_deadeye(value.thief_deadeye);
    should_write |= value.thief_deadeye;
    proto_specialization_filter->set_warrior_spellbreaker(value.warrior_spellbreaker);
    should_write |= value.warrior_spellbreaker;
    proto_specialization_filter->set_elementalist_catalyst(value.elementalist_catalyst);
    should_write |= value.elementalist_catalyst;
    proto_specialization_filter->set_engineer_mechanist(value.engineer_mechanist);
    should_write |= value.engineer_mechanist;
    proto_specialization_filter->set_guardian_willbender(value.guardian_willbender);
    should_write |= value.guardian_willbender;
    proto_specialization_filter->set_mesmer_virtuoso(value.mesmer_virtuoso);
    should_write |= value.mesmer_virtuoso;
    proto_specialization_filter->set_necromancer_harbinger(value.necromancer_harbinger);
    should_write |= value.necromancer_harbinger;
    proto_specialization_filter->set_ranger_untamed(value.ranger_untamed);
    should_write |= value.ranger_untamed;
    proto_specialization_filter->set_revenant_vindicator(value.revenant_vindicator);
    should_write |= value.revenant_vindicator;
    proto_specialization_filter->set_thief_specter(value.thief_specter);
    should_write |= value.thief_specter;
    proto_specialization_filter->set_warrior_bladesworn(value.warrior_bladesworn);
    should_write |= value.warrior_bladesworn;
    proto_specialization_filter->set_elementalist_air(value.elementalist_air);
    should_write |= value.elementalist_air;
    proto_specialization_filter->set_elementalist_arcane(value.elementalist_arcane);
    should_write |= value.elementalist_arcane;
    proto_specialization_filter->set_elementalist_earth(value.elementalist_earth);
    should_write |= value.elementalist_earth;
    proto_specialization_filter->set_elementalist_fire(value.elementalist_fire);
    should_write |= value.elementalist_fire;
    proto_specialization_filter->set_elementalist_water(value.elementalist_water);
    should_write |= value.elementalist_water;
    proto_specialization_filter->set_engineer_alchemy(value.engineer_alchemy);
    should_write |= value.engineer_alchemy;
    proto_specialization_filter->set_engineer_explosives(value.engineer_explosives);
    should_write |= value.engineer_explosives;
    proto_specialization_filter->set_engineer_firearms(value.engineer_firearms);
    should_write |= value.engineer_firearms;
    proto_specialization_filter->set_engineer_inventions(value.engineer_inventions);
    should_write |= value.engineer_inventions;
    proto_specialization_filter->set_engineer_tools(value.engineer_tools);
    should_write |= value.engineer_tools;
    proto_specialization_filter->set_guardian_honor(value.guardian_honor);
    should_write |= value.guardian_honor;
    proto_specialization_filter->set_guardian_radiance(value.guardian_radiance);
    should_write |= value.guardian_radiance;
    proto_specialization_filter->set_guardian_valor(value.guardian_valor);
    should_write |= value.guardian_valor;
    proto_specialization_filter->set_guardian_virtues(value.guardian_virtues);
    should_write |= value.guardian_virtues;
    proto_specialization_filter->set_guardian_zeal(value.guardian_zeal);
    should_write |= value.guardian_zeal;
    proto_specialization_filter->set_mesmer_chaos(value.mesmer_chaos);
    should_write |= value.mesmer_chaos;
    proto_specialization_filter->set_mesmer_domination(value.mesmer_domination);
    should_write |= value.mesmer_domination;
    proto_specialization_filter->set_mesmer_dueling(value.mesmer_dueling);
    should_write |= value.mesmer_dueling;
    proto_specialization_filter->set_mesmer_illusions(value.mesmer_illusions);
    should_write |= value.mesmer_illusions;
    proto_specialization_filter->set_mesmer_inspiration(value.mesmer_inspiration);
    should_write |= value.mesmer_inspiration;
    proto_specialization_filter->set_necromancer_blood_magic(value.necromancer_blood_magic);
    should_write |= value.necromancer_blood_magic;
    proto_specialization_filter->set_necromancer_curses(value.necromancer_curses);
    should_write |= value.necromancer_curses;
    proto_specialization_filter->set_necromancer_death_magic(value.necromancer_death_magic);
    should_write |= value.necromancer_death_magic;
    proto_specialization_filter->set_necromancer_soul_reaping(value.necromancer_soul_reaping);
    should_write |= value.necromancer_soul_reaping;
    proto_specialization_filter->set_necromancer_spite(value.necromancer_spite);
    should_write |= value.necromancer_spite;
    proto_specialization_filter->set_ranger_beastmastery(value.ranger_beastmastery);
    should_write |= value.ranger_beastmastery;
    proto_specialization_filter->set_ranger_marksmanship(value.ranger_marksmanship);
    should_write |= value.ranger_marksmanship;
    proto_specialization_filter->set_ranger_nature_magic(value.ranger_nature_magic);
    should_write |= value.ranger_nature_magic;
    proto_specialization_filter->set_ranger_skirmishing(value.ranger_skirmishing);
    should_write |= value.ranger_skirmishing;
    proto_specialization_filter->set_ranger_wilderness_survival(value.ranger_wilderness_survival);
    should_write |= value.ranger_wilderness_survival;
    proto_specialization_filter->set_revenant_corruption(value.revenant_corruption);
    should_write |= value.revenant_corruption;
    proto_specialization_filter->set_revenant_devastation(value.revenant_devastation);
    should_write |= value.revenant_devastation;
    proto_specialization_filter->set_revenant_invocation(value.revenant_invocation);
    should_write |= value.revenant_invocation;
    proto_specialization_filter->set_revenant_retribution(value.revenant_retribution);
    should_write |= value.revenant_retribution;
    proto_specialization_filter->set_revenant_salvation(value.revenant_salvation);
    should_write |= value.revenant_salvation;
    proto_specialization_filter->set_thief_acrobatics(value.thief_acrobatics);
    should_write |= value.thief_acrobatics;
    proto_specialization_filter->set_thief_critical_strikes(value.thief_critical_strikes);
    should_write |= value.thief_critical_strikes;
    proto_specialization_filter->set_thief_deadly_arts(value.thief_deadly_arts);
    should_write |= value.thief_deadly_arts;
    proto_specialization_filter->set_thief_shadow_arts(value.thief_shadow_arts);
    should_write |= value.thief_shadow_arts;
    proto_specialization_filter->set_thief_trickery(value.thief_trickery);
    should_write |= value.thief_trickery;
    proto_specialization_filter->set_warrior_arms(value.warrior_arms);
    should_write |= value.warrior_arms;
    proto_specialization_filter->set_warrior_defense(value.warrior_defense);
    should_write |= value.warrior_defense;
    proto_specialization_filter->set_warrior_discipline(value.warrior_discipline);
    should_write |= value.warrior_discipline;
    proto_specialization_filter->set_warrior_strength(value.warrior_strength);
    should_write |= value.warrior_strength;
    proto_specialization_filter->set_warrior_tactics(value.warrior_tactics);
    should_write |= value.warrior_tactics;
    if (should_write) {
        setter(proto_specialization_filter);
    }
}
