---
attribute_category: Schedule
fields:
    - name: Schedule
      description: A description of when to hide or show an object based on a realtime schedule.
      type: String
      applies_to: [Icon, Trail]
      xml_fields: [Schedule]
      protobuf_field: bhdraft__schedule
      compatability: [BlishHUD]

    - name: Schedule Duration
      description: Combined with Schedule to determine how long after the object is shown before it is hidden again.
      type: Float32
      applies_to: [Icon, Trail]
      xml_fields: [ScheduleDuration]
      protobuf_field: bhdraft__schedule_duration
      compatability: [BlishHUD]
---

Notes
=====
https://blishhud.com/docs/markers/attributes/schedule
