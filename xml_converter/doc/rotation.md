---
attribute_category: Rotation
fields:
    - name: Rotation
      type: CompoundValue
      applies_to: ["Icon"]
      xml_fields: ["Rotation"]
      compatability: [TacO, Burrito, BlishHUD]
      description: "Euler X Y Z rotation."
      protobuf_field: euler_rotation
      components:
          - name: X Rotation
            type: Float32
            xml_fields: [RotateX]
            protobuf_field: "x"
            compatability: [TacO, Burrito, BlishHUD]
            description: The X Component of the Euler rotation.
          
          - name: Y Rotation
            type: Float32
            xml_fields: [RotateY]
            protobuf_field: "y"
            compatability: [TacO, Burrito, BlishHUD]
            description: The Y Component of the Euler rotation.
          
          - name: Z Rotation
            type: Float32
            xml_fields: [RotateZ]
            protobuf_field: "z"
            compatability: [TacO, Burrito, BlishHUD]
            description: The Z Component of the Euler rotation.
      xml_export: "Parent Only"
---

Notes
=====
https://blishhud.com/docs/markers/attributes/rotate
