---
name: Menu ID
type: Custom
class: UniqueId
xml_fields: [ID, MenuID]
applies_to: [Category]
protobuf_field: id
---

Notes
=====

A 128bit long Unique ID.

If the ID is not present when converting from XML, the ID is created as a hash of the category's "name" and the names of the categories parents.
