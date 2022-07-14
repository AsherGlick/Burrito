---
attribute_category: Species Filter
fields:
    - name: Species Filter
      type: MultiflagValue
      applies_to: [Icon, Trail]
      compatability: [TacO, BlishHUD, Burrito]
      xml_fields: [Race, Species]
      protobuf_field: species_filter
      description: "The player must be player a character of one of the species in this list."
      flags:
        asura: [asura]
        charr: [charr]
        human: [human]
        norn: [norn]
        sylvari: [sylvari]
---

Notes
=====
https://blishhud.com/docs/markers/attributes/race