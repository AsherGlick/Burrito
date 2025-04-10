---
name: Reset Length
type: Float32
applies_to: [Icon]
xml_fields: [ResetLength]
exclude_from_protobuf: True
protobuf_field: trigger.reset_length
---
When using behavior 4 (reappear after timer) this value defines, in seconds, the duration until the marker is reset after being activated.
Notes
=====

https://blishhud.com/docs/markers/attributes/resetlength






PARSEABLE_VAR(reset_offset, float, "ResetOffset") // Depricated

PARSEABLE_VAR(has_countdown, bool, "HasCountdown")