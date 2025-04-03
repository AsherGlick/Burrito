---
name: Constant Size On Map
type: Boolean
applies_to: [Icon]
xml_fields: [ScaleOnMapWithZoom]
exclude_from_protobuf: True
protobuf_field: constant_size_on_map
custom_functions:
  read.xml:
    function: Attribute::InvertBool::from_xml_attribute
    side_effects: []
  write.xml:
    function: Attribute::InvertBool::to_xml_attribute
    side_effects: []
---
Scale the size of the object to be the same scale as the map.

Notes
=====

https://blishhud.com/docs/markers/attributes/scaleonmapwithzoom

