---
attribute_category: Achievement
fields:
    - name: Achievement ID
      type: Int32
      applies_to: [Icon, Trail]
      compatability: [TacO, BlishHUD, Burrito]
      xml_fields: ["AchievementId"]
      protobuf_field: "achievement_id"
      description: "An achivement that, if completed, will hide this marker."

    - name: Achievement Bitmask
      type: Fixed32
      applies_to: [Icon, Trail]
      compatability: [TacO, BlishHUD, Burrito]
      xml_fields: ["AchievementBit"]
      protobuf_field: "achievement_bit"
      description: "A portion of an achievement that, if completed, will hide this marker."
---

Notes
=====

https://blishhud.com/docs/markers/attributes/achievement