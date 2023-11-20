---
name: Category
type: Custom
class: MarkerCategory
applies_to: [Icon, Trail]
xml_fields: [Type, Category]
protobuf_field: category
custom_functions:
  read.proto:
    function: do_nothing
    side_effects: []
  write.proto:
    function: do_nothing
    side_effects: []
---
The category this object belongs to.

Notes
=====
https://blishhud.com/docs/markers/attributes/type
