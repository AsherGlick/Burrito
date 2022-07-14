---
attribute_category: Scale
fields:
    - name: Render on Minimap
      description: Allows or Prevents this object from being rendered on the minimap aka compass.
      type: Boolean
      applies_to: [Icon, Trail]
      xml_fields: [MinimapVisibility, BHMinimapVisibility]
      protobuf_field: __tentative__render_on_minimap
      compatability: [TacO, BlishHUD, Burrito]

    - name: Render on Map
      description: Allows or Prevents this object from being rendered on the world map.
      type: Boolean
      applies_to: [Icon, Trail]
      xml_fields: [MapVisibility, BHMapVisibility]
      protobuf_field: __tentative__render_on_map
      compatability: [TacO, BlishHUD, Burrito]

    - name: Render Ingame
      description: Allows or Prevents this object from being rendered in the 3D game space.
      type: Boolean
      applies_to: [Icon, Trail]
      xml_fields: [IngameVisibility, BHIngameVisibility]
      protobuf_field: __tentative__render_ingame
      compatability: [TacO, BlishHUD, Burrito]

    - name: Render Ingame
      description: Allows or Prevents this object from being rendered in the 3D game space.
      type: Boolean
      applies_to: [Icon, Trail]
      xml_fields: [IngameVisibility, BHIngameVisibility]
      protobuf_field: __tentative__render_ingame
      compatability: [TacO, BlishHUD, Burrito]

    - name: Map Display Size
      description: The size, in pixels, that the marker should be shown on the minimap or fullscreen map.
      type: Int32
      applies_to: [Icon]
      xml_fields: [MapDisplaySize]
      protobuf_field: map_display_size
      compatability: [TacO, BlishHUD, Burrito]

    - name: Scale on Map With Zoom
      description: Scale the size of the object to be the same scale as the map.
      type: Boolean
      applies_to: [Icon]
      xml_fields: [ScaleOnMapWithZoom]
      protobuf_field: scale_on_map_with_zoom
      compatability: [TacO, BlishHUD, Burrito]

    - name: Cull
      description: Cull one of the sides of an Icon or Trail
      applies_to: [Icon, Trail]
      xml_fields: [Cull]
      protobuf_field: cull
      type: Enum
      compatability: [TacO, BlishHUD, Burrito]
      values:
        none: ["None"]
        clockwise: ["Clockwise"]
        counter_clockwise: ["CounterClockwise"]

---

Notes
=====
https://blishhud.com/docs/markers/attributes/visibility

We want to figure out a way to invert this value becuase a "false" value is the default value inside a protobuf and if we set the default as "true" then we have to write this field for every object. This inversion will need to be present in the code generator and that might take a bit to design and implement.

https://blishhud.com/docs/markers/attributes/mapdisplaysize
https://blishhud.com/docs/markers/attributes/scaleonmapwithzoom
 https://blishhud.com/docs/markers/attributes/cull

