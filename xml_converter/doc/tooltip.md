---
attribute_category: Scale
fields:
    - name: Tooltip Name
      description: The name of the tooltip when hovering over the minimap.
      type: String
      applies_to: [Icon]
      xml_fields: [TipName]
      protobuf_field: tip_name
      compatability: [TacO, BlishHUD, Burrito]

    - name: Tooltip Description
      description: The full text of the tooltip.
      type: String
      applies_to: [Icon, Category]
      xml_fields: [TipDescription]
      protobuf_field: tip_description
      compatability: [TacO, BlishHUD, Burrito]
---

Notes
=====


https://blishhud.com/docs/markers/attributes/tip
