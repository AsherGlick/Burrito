---
name: Render Ingame
type: Boolean
applies_to: [Icon, Trail]
xml_fields: [IngameVisibility, BHIngameVisibility]
protobuf_field: __tentative__render_ingame
compatability: [TacO, BlishHUD, Burrito]

---

Allows or Prevents this object from being rendered in the 3D game space.

Notes
=====

We want to figure out a way to invert this value becuase a "false" value is the default value inside a protobuf and if we set the default as "true" then we have to write this field for every object. This inversion will need to be present in the code generator and that might take a bit to design and implement.

