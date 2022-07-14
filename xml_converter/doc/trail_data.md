---
attribute_category: Trail Data
fields:
    - name: Trail Data
      type: Custom
      class: TrailData
      xml_fields: ["TrailData"]
      protobuf_field: traildata
      side_effects: [Map ID]
      applies_to: [Trail]
      compatability: [TacO, BlishHUD, Burrito]
      description: A binary coordinate array for the path of a trail.

    - name: Trail Data Map ID
      type: Custom
      class: TrailDataMapId
      xml_fields: ["TrailData"]
      protobuf_field: map_id
      applies_to: [Trail]
      compatability: [TacO, BlishHUD, Burrito]
      description: The map id is embedded inside of the trail binary for some ungodly reason.
---


Notes
=====



TrailData


GetFromXML(xml_node)
WriteToXML(self) # not quite right.


GetFromProtobuf(protobuf_node_object) # not quite right
SetProtobuf() # not quite right



the return value of the set should be the value that needs to be set.
But also we can just pass in the number of variables to be "set" or "get"



so really
  GetFromProtobuf(arg1type* argument1, arg2type* argument2)
so really 
  SetProtobuf(arg1type* argument1, arg2type* argument2)