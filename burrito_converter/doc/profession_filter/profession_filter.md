---
name: Profession Filter
type: MultiflagValue
applies_to: [Icon, Trail]
xml_fields: [Profession]
protobuf_field: profession_filter
flags:
  guardian: [Guardian]
  warrior: [Warrior]
  engineer: [Engineer]
  ranger: [Ranger]
  thief: [Thief]
  elementalist: [Elementalist]
  mesmer: [Mesmer]
  necromancer: [Necromancer]
  revenant: [Revenant]
---
The player must be player a character of one of the professions in this list.

Notes
=====
https://blishhud.com/docs/markers/attributes/profession

PARSEABLE_VAR(profession_filter, ProfessionFilter, "Profession")
