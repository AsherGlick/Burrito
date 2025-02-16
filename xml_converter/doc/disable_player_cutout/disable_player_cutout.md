---
name: Disable Player Cutout
type: Boolean
applies_to: [Icon, Trail]
xml_fields: [CanFade]
exclude_from_protobuf: True
protobuf_field: disable_player_cutout
custom_functions:
  read.xml:
    function: Attribute::InvertBool::from_xml_attribute
    side_effects: []
  write.xml:
    function: Attribute::InvertBool::to_xml_attribute
    side_effects: []
---
A flag to determine if an object can be made transparent when it is possibly occluding the player.

This value is inverted between the guildpoint format value of "Disable Player Cutout" and the TacO xml format of "Can Fade"


Notes
=====
https://blishhud.com/docs/markers/attributes/canfade
