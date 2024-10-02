---
name: Render Ingame
type: Boolean
applies_to: [Icon, Trail]
xml_fields: [IngameVisibility, BHIngameVisibility]
protobuf_field: is_hidden_ingame
custom_functions:
  read.xml:
    function: inverted_xml_attribute_to_bool
    side_effects: []
  write.xml:
    function: bool_to_inverted_xml_attribute
    side_effects: []
---

Allows or Prevents this object from being rendered in the 3D game space.

Notes
=====

https://gw2pathing.com/docs/marker-dev/attributes/visibility

