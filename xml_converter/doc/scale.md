---
attribute_category: Scale
fields:
    - name: Trail Scale
      description: Adjusts the width of a trail mesh as a multiplier to the default width.
      type: Float32
      applies_to: [Trail]
      xml_fields: [TrailScale]
      protobuf_field: scale
      compatability: [TacO, BlishHUD, Burrito]

    - name: Icon Size
      description: (Unclear) Some value representation of how large an icon should be. Is this a scale multiplier or a fixed size value? How does this relate to MinSize and MaxSize.
      type: Float32
      applies_to: [Icon]
      xml_fields: [IconSize]
      protobuf_field: __tentative__scale
      compatability: [TacO, BlishHUD, Burrito]
---

Notes
=====
https://blishhud.com/docs/markers/attributes/trailscale/
https://blishhud.com/docs/markers/attributes/iconfile
https://blishhud.com/docs/markers/attributes/color
