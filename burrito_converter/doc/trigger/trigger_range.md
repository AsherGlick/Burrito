---
name: Trigger Range
type: Float32
applies_to: [Icon]
xml_fields: [TriggerRange, InfoRange]
#151 This field should be set to 0 in the protobuf when equal to the default value
exclude_from_protobuf: True
protobuf_field: trigger.range
---

Used to indicate a trigger range. This attribute is used by multiple other attributes to define a distance from the marker in which those attributes will activate their functionality or behavior.

Notes
=====

https://blishhud.com/docs/markers/attributes/triggerrange





PARSEABLE_VAR(reset_offset, float, "ResetOffset") // Depricated

PARSEABLE_VAR(has_countdown, bool, "HasCountdown")