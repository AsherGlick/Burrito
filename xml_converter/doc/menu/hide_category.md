---
name: Hide Category
type: Boolean
applies_to: [Category]
xml_fields: [DefaultToggle]
protobuf_field: hide_category
custom_functions:
  read.xml:
    function: inverted_xml_attribute_to_bool
    side_effects: []
  write.xml:
    function: bool_to_inverted_xml_attribute
    side_effects: []
---

Notes
=====
If the category should be shown or hidden on or off by default.

https://blishhud.com/docs/markers/attributes/defaulttoggle


