---
name: Achievement Bit Index
type: Int32
applies_to: [Icon, Trail]
xml_fields: ["AchievementBit", "AchievementBitIndex"]
exclude_from_protobuf: True
protobuf_field: achievement_bit_index
examples: ["0", "1", "2", "14"]
---
A portion of the achievement that will hide this marker if it is completed.

This value corrisponds to an `index` of a "bit" of an achivment. For example we can look at the achivement "RGB" which has the id `2655`.

[https://api.guildwars2.com/v2/achievements?ids=2655](https://api.guildwars2.com/v2/achievements?ids=2655)

```json
[
  {
    "id": 2655,
    "name": "RGB",
    "description": "",
    "requirement": "Obtain each pylon attunement once, and defeat the Vale Guardian.",
    "locked_text": "",
    "type": "Default",
    "flags": ["Permanent"],
    "bits": [
      {"type": "Text", "text": "Red Attunement"},
      {"type": "Text", "text": "Green Attunement"},
      {"type": "Text", "text": "Blue Attunement"}
    ],
    "tiers": [{"count": 3, "points": 1}]
  }
]
```
`Red Attunement` is the first one, so because we start with 0 it has the index **"0"**  
`Green Attunement` is the second, so it has the index **"1"**  
`Blue Attunement` is the third, so it has the index **"2"**  

If we wanted to hide this marker when "Green Attunment" had been completed we would set the Achievement Bit Index to `1`



External Resources
--------------------------------------------------------------------------------
* [https://gw2pathing.com/docs/marker-dev/attributes/achievement/](https://gw2pathing.com/docs/marker-dev/attributes/achievement/)


