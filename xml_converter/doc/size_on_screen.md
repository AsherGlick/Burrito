---
attribute_category: Size On Screen
fields:
    - name: Minimum Size on Screen
      description: The smallest width/height of this icon on the screen.
      type: Int32
      applies_to: [Icon]
      xml_fields: [MinSize]
      protobuf_field: minimum_size_on_screen
      compatability: [TacO, BlishHUD, Burrito]

    - name: Maximum Size On Screen
      description: The largest width/height of this icon on the screen.
      type: Int32
      applies_to: [Icon]
      xml_fields: [MaxSize]
      protobuf_field: maximum_size_on_screen
      compatability: [TacO, BlishHUD, Burrito]
---

Notes
=====
https://blishhud.com/docs/markers/attributes/size
