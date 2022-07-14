---
attribute_category: Category
fields:
    - name: Default Visibility
      description: If the category should be shown or hidden on or off by default.
      type: Boolean
      applies_to: [Category]
      xml_fields: [DefaultToggle]
      protobuf_field: default_visibility
      compatability: [TacO, BlishHUD, Burrito]

    - name: Display Name
      description: A human readable name of this category.
      type: String
      applies_to: [Category]
      xml_fields: [DisplayName]
      protobuf_field: display_name
      compatability: [TacO, BlishHUD, Burrito]

    - name: Name
      description: A machine readable name of this category.
      type: String
      applies_to: [Category]
      xml_fields: [Name]
      protobuf_field: name
      compatability: [TacO, BlishHUD, Burrito]

    - name: Is Seperator
      description: Determines if this element is a seperator not a category.
      type: Boolean
      applies_to: [Category]
      xml_fields: [IsSeperator]
      protobuf_field: is_seperator
      compatability: [TacO, BlishHUD, Burrito]


---

Notes
=====


https://blishhud.com/docs/markers/attributes/defaulttoggle
https://blishhud.com/docs/markers/attributes/displayname
https://blishhud.com/docs/markers/attributes/isseparator

