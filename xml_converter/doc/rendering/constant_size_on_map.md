---
name: Constant Size On Map
type: Boolean
applies_to: [Icon]
xml_fields: [ScaleOnMapWithZoom]
protobuf_field: constant_size_on_map
custom_functions:
  read.xml:
    function: inverted_xml_attribute_to_bool
    side_effects: []
  write.xml:
    function: bool_to_inverted_xml_attribute
    side_effects: []
---
Scale the size of the object to be the same scale as the map.

Notes
=====

https://blishhud.com/docs/markers/attributes/scaleonmapwithzoom

