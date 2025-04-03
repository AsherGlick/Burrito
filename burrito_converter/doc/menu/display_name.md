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
    function: Attribute::NameAndDisplayname::from_proto_field
    side_effects: [Name]
  write.proto:
    function: Attribute::NameAndDisplayname::to_proto_field
    side_effects: [Name]

---
A human readable name of this category.

Notes
=====
https://blishhud.com/docs/markers/attributes/displayname


