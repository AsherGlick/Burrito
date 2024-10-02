---
name: Map Display Size
type: Int32
applies_to: [Icon, Trail] 
xml_fields: [MapDisplaySize]
#151 This field should be set to 0 in the protobuf when equal to the default value
protobuf_field: null #map_display_size
---
The size, in pixels, that the marker should be shown on the minimap or fullscreen map.
When applied to a Trail, affects the width of the trail shown on the minimap or fullscreen map.

Notes
=====
https://blishhud.com/docs/markers/attributes/mapdisplaysize


