---
attribute_category: Festival Filter
fields:
    - name: Festival Filter
      type: MultiflagValue
      applies_to: [Icon, Trail]
      compatability: [TacO, BlishHUD, Burrito]
      xml_fields: [Festival]
      protobuf_field: festival_filter
      description: "One of these festivals must be occuring for the markers to apear."
      flags:
        dragonbash: [DragonBash]
        festival_of_the_four_winds: [FestivalOfTheFourWinds]
        halloween: [Halloween]
        lunar_new_year: [LunarNewYear]
        super_adventure_festival: [SuperAdventureFestival, SuperAdventureBox]
        wintersday: [Wintersday]
        none: [None]

---

Notes
=====
https://blishhud.com/docs/markers/attributes/festival
