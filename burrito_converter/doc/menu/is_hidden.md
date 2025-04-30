---
name: Is Hidden
type: Boolean
applies_to: [Category]
xml_fields: [DefaultToggle]
exclude_from_protobuf: True
protobuf_field: is_hidden
custom_functions:
  read.xml:
    function: Attribute::InvertBool::from_xml_attribute
    side_effects: []
  write.xml:
    function: Attribute::InvertBool::to_xml_attribute
    side_effects: []
---

Notes
=====
If the category should be shown or hidden on or off by default.

https://blishhud.com/docs/markers/attributes/defaulttoggle


