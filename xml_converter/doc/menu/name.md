---   
name: Name
type: String
applies_to: [Category]
xml_fields: [Name]
protobuf_field: name
custom_functions:
  read.proto:
    function: do_nothing
    side_effects: []
  write.proto:
    function: do_nothing
    side_effects: []
---

Notes
=====
A machine readable name of this category.

https://blishhud.com/docs/markers/attributes/displayname


