---
attribute_category: Fade
fields:
    - name: Distance Fade Start
      description: Any part of the object that is farther then this value will begin to alpha fade gradually until it reaches 0 alpha at Distance Fade End.
      type: Float32
      applies_to: [Icon, Trail]
      xml_fields: [FadeNear, DistanceFadeStart]
      protobuf_field: distance_fade_start
      compatability: [TacO, BlishHUD, Burrito]

    - name: Distance Fade End
      description: Any part of the object that is farther then this value will be at 
      type: Float32
      applies_to: [Icon, Trail]
      xml_fields: [FadeFar, DistanceFadeEnd]
      protobuf_field: distance_fade_end
      compatability: [TacO, BlishHUD, Burrito]
---

Notes
=====
https://blishhud.com/docs/markers/attributes/fade
