#pragma once

#include <functional>
#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../state_structs/proto_reader_state.hpp"
#include "../state_structs/proto_writer_state.hpp"
#include "../state_structs/xml_reader_state.hpp"
#include "../state_structs/xml_writer_state.hpp"

class XMLError;
namespace guildpoint {
class SpecializationFilter;
}

namespace Attribute::SpecializationFilter {

class SpecializationFilter {
 public:
    bool elementalist_air;
    bool elementalist_arcane;
    bool elementalist_catalyst;
    bool elementalist_earth;
    bool elementalist_fire;
    bool elementalist_tempest;
    bool elementalist_water;
    bool elementalist_weaver;
    bool engineer_alchemy;
    bool engineer_explosives;
    bool engineer_firearms;
    bool engineer_holosmith;
    bool engineer_inventions;
    bool engineer_mechanist;
    bool engineer_scrapper;
    bool engineer_tools;
    bool guardian_dragonhunter;
    bool guardian_firebrand;
    bool guardian_honor;
    bool guardian_radiance;
    bool guardian_valor;
    bool guardian_virtues;
    bool guardian_willbender;
    bool guardian_zeal;
    bool mesmer_chaos;
    bool mesmer_chronomancer;
    bool mesmer_domination;
    bool mesmer_dueling;
    bool mesmer_illusions;
    bool mesmer_inspiration;
    bool mesmer_mirage;
    bool mesmer_virtuoso;
    bool necromancer_blood_magic;
    bool necromancer_curses;
    bool necromancer_death_magic;
    bool necromancer_harbinger;
    bool necromancer_reaper;
    bool necromancer_scourge;
    bool necromancer_soul_reaping;
    bool necromancer_spite;
    bool ranger_beastmastery;
    bool ranger_druid;
    bool ranger_marksmanship;
    bool ranger_nature_magic;
    bool ranger_skirmishing;
    bool ranger_soulbeast;
    bool ranger_untamed;
    bool ranger_wilderness_survival;
    bool revenant_corruption;
    bool revenant_devastation;
    bool revenant_herald;
    bool revenant_invocation;
    bool revenant_renegade;
    bool revenant_retribution;
    bool revenant_salvation;
    bool revenant_vindicator;
    bool thief_acrobatics;
    bool thief_critical_strikes;
    bool thief_daredevil;
    bool thief_deadeye;
    bool thief_deadly_arts;
    bool thief_shadow_arts;
    bool thief_specter;
    bool thief_trickery;
    bool warrior_arms;
    bool warrior_berserker;
    bool warrior_bladesworn;
    bool warrior_defense;
    bool warrior_discipline;
    bool warrior_spellbreaker;
    bool warrior_strength;
    bool warrior_tactics;

    virtual std::string classname() {
        return "SpecializationFilter";
    }
};
void from_xml_attribute(
    rapidxml::xml_attribute<>* input,
    std::vector<XMLError*>* errors,
    XMLReaderState* state,
    SpecializationFilter* value,
    bool* is_set
);

void to_xml_attribute(
    XMLWriterState* state,
    const SpecializationFilter* value,
    std::function<void(std::string)> setter
);

void from_proto_field(
    guildpoint::SpecializationFilter input,
    ProtoReaderState* state,
    SpecializationFilter* value,
    bool* is_set
);

void to_proto_field(
    SpecializationFilter value,
    ProtoWriterState* state,
    std::function<void(guildpoint::SpecializationFilter*)> setter
);

}  // namespace Attribute::SpecializationFilter
