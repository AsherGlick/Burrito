---
name: Reset Behavior
type: Enum
applies_to: [Icon]
xml_fields: [Behavior]
exclude_from_protobuf: True
protobuf_field: trigger.reset_behavior
values:
  always_visible: ["0", "always_visible"]
  map_change: ["1", "map_change"]
  daily_reset: ["2", "daily_reset"]
  never: ["3", "never"]
  timer: ["4", "timer"]
  map_reset: ["5", "map_reset"]
  instance_change: ["6", "instance_change"]
  daily_reset_per_character: ["7", "daily_reset_per_character"]

  weekly_reset: ["101", "weekly_reset"]
---
A behavior around triggers and retriggering.

Notes
=====

https://blishhud.com/docs/markers/attributes/behavior





PARSEABLE_VAR(reset_offset, float, "ResetOffset") // Depricated

PARSEABLE_VAR(has_countdown, bool, "HasCountdown")