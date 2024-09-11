---
name: Render on Minimap
type: Boolean
applies_to: [Icon, Trail]
xml_fields: [MinimapVisibility, BHMinimapVisibility]
protobuf_field: is_hidden_on_minimap
custom_functions:
  read.xml:
    function: inverted_xml_attribute_to_bool
    side_effects: []
  write.xml:
    function: bool_to_inverted_xml_attribute
    side_effects: []
---

Allows or Prevents this object from being rendered on the minimap aka compass.

Notes
=====
