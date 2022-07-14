---
attribute_category: Mount Filter
fields:
    - name: Mount Filter
      type: MultiflagValue
      applies_to: [Icon, Trail]
      compatability: [TacO, BlishHUD, Burrito]
      xml_fields: ["Mount"]
      protobuf_field: mount_filter
      description: "A player must be on one of the mounts in the list to be able to see the objects."
      flags:
          raptor: ["Raptor"]
          springer: ["Springer"]
          skimmer: ["Skimmer"]
          jackal: ["Jackal"]
          griffon: ["Griffon"]
          roller_beetle: ["RollerBeetle"]
          warclaw: ["Warclaw"]
          skyscale: ["Skyscale"]
          skiff: ["Skiff"]
          seige_turtle: ["SeigeTurtle"]


---

Notes
=====
https://blishhud.com/docs/markers/attributes/mount
