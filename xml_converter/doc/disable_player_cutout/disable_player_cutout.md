---
name: Disable Player Cutout
type: Boolean
applies_to: [Icon, Trail]
xml_fields: [CanFade]
protobuf_field: disable_player_cutout
custom_functions:
  read.xml:
    function: inverted_xml_attribute_to_bool
    side_effects: []
  write.xml:
    function: bool_to_inverted_xml_attribute
    side_effects: []
---
A flag to determine if an object can be made transparent when it is possibly occluding the player.

This value is inverted between the guildpoint format value of "Disable Player Cutout" and the TacO xml format of "Can Fade"


Notes
=====
https://blishhud.com/docs/markers/attributes/canfade
