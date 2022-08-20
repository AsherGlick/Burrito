---
name: Position
type: CompoundValue
applies_to: ["Icon"]
xml_fields: ["Position"]
compatability: [TacO, Burrito, BlishHUD]
protobuf_field: position
components:
    
    - name: X Position
      type: Float32
      xml_fields: [XPos, PositionX]
      protobuf_field: "x"
      compatability: [TacO, Burrito, BlishHUD]
      
    
    - name: Y Position
      type: Float32
      xml_fields: [YPos, PositionY]
      protobuf_field: "y"
      compatability: [TacO, Burrito, BlishHUD]
      
    
    - name: Z Position
      type: Float32
      xml_fields: [ZPos, PositionZ]
      protobuf_field: "z"
      compatability: [TacO, Burrito, BlishHUD]
      
xml_export: "Children Only"
---
An XYZ location of a point in the game world.

Notes
=====
https://blishhud.com/docs/markers/attributes/position
https://blishhud.com/docs/markers/attributes/heightoffset
