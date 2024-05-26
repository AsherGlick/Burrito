---
name: Map ID
type: Int32
applies_to: [Icon, Trail]
xml_fields: [MapID]
protobuf_field: map_id
examples:
  - "24"
  - "50"
  - "1509"
---
The Map which this marker should be displayed on.

Finding the Map ID
==================
Method 1: GameInfo Plugin
---
The GameInfo plugin displays some useful game information on your screen about the character. One of those is the map the character is currently on

Method 2: [Guild Wars 2 Wiki][2] infobox
---
the InfoBox on the right hand side of map wiki pages has a field titled API followed by a number.

For example, the [Gendarran Fields][1] wiki page has the API value of `24`. Gendarran Fields' Map ID is `24`.

![Gendarran Fields InfoBox](./gendarran_fields_infobox.png)


Quirks
======
* Map ID is also stored within the `.trl` files and may overwrite any other Map IDs set earlier in the node.


References
=========
* [BlishHUD MapID Marker Docs](https://blishhud.com/docs/markers/attributes/mapid)
* [TacO Marker Pack Guide](https://www.gw2taco.com/2016/01/how-to-create-your-own-marker-pack.html)


[1]: https://wiki.guildwars2.com/wiki/Gendarran_Fields
[2]: https://wiki.guildwars2.com