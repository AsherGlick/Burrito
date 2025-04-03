---
name: Render on Map
type: Boolean
applies_to: [Icon, Trail]
xml_fields: [MapVisibility, BHMapVisibility]
protobuf_field: is_hidden_on_map
custom_functions:
  read.xml:
    function: Attribute::InvertBool::from_xml_attribute
    side_effects: []
  write.xml:
    function: Attribute::InvertBool::to_xml_attribute
    side_effects: []
---
Allows or Prevents this object from being rendered on the world map.

Notes
=====

