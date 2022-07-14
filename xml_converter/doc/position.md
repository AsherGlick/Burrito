---
attribute_category: Position
fields:
    - name: Position
      type: CompoundValue
      applies_to: ["Icon"]
      xml_fields: ["Position"]
      compatability: [TacO, Burrito, BlishHUD]
      description: "An XYZ location of a point in the game world."
      protobuf_field: position
      components:
          
          - name: X Position
            type: Float32
            xml_fields: [XPos, PositionX]
            protobuf_field: "x"
            compatability: [TacO, Burrito, BlishHUD]
            description: The X Component of the Position value
          
          - name: Y Position
            type: Float32
            xml_fields: [YPos, PositionX]
            protobuf_field: "y"
            compatability: [TacO, Burrito, BlishHUD]
            description: The Y Component of the Position value
          
          - name: Z Position
            type: Float32
            xml_fields: [ZPos, PositionZ]
            protobuf_field: "z"
            compatability: [TacO, Burrito, BlishHUD]
            description: The Z Component of the Position value
      xml_export: "Children Only"

    - name: Height Offset
      type: Float32
      applies_to: ["Icon"]
      xml_fields: ["HeightOffset"]
      compatability: [TacO, Burrito, BlishHUD]
      description: A vertical offset of the object from the recorded position.
      protobuf_field: height_offset
---

Notes
=====
https://blishhud.com/docs/markers/attributes/position
https://blishhud.com/docs/markers/attributes/heightoffset
