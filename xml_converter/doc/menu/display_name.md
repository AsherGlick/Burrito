---
name: Display Name
type: String
applies_to: [Category]
xml_fields: [DisplayName]
protobuf_field: name
examples:
  - "My Category"
  - "Tribulation Mode 203"
custom_functions:
  read.proto:
    function: proto_display_name_to_display_name_and_name
    side_effects: [Name]
  write.proto:
    function: display_name_and_name_to_proto_display_name
    side_effects: [Name]

---
A human readable name of this category.

Notes
=====
https://blishhud.com/docs/markers/attributes/displayname


