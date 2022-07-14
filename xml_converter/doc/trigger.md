---
attribute_category: Trigger
fields:
    - name: Auto Trigger
      description: Any triggers will be triggered automatially when the player enters within range instead of being triggered.
      type: Boolean
      applies_to: [Icon]
      xml_fields: [AutoTrigger]
      protobuf_field: trigger.auto_trigger
      compatability: [TacO, BlishHUD, Burrito]

    - name: Trigger Range
      description: Used to indicate a trigger range. This attribute is used by multiple other attributes to define a distance from the marker in which those attributes will activate their functionality or behavior.
      type: Float32
      applies_to: [Icon]
      xml_fields: [TriggerRange, InfoRange]
      protobuf_field: trigger.range
      compatability: [TacO, BlishHUD, Burrito]

    - name: Reset Length
      description: When using behavior 4 (reappear after timer) this value defines, in seconds, the duration until the marker is reset after being activated.
      type: Float32
      applies_to: [Icon]
      xml_fields: [ResetLength]
      protobuf_field: trigger.reset_length
      compatability: [TacO, BlishHUD, Burrito]

    - name: Show Category
      description: Show the specified category when triggered.
      type: Custom
      class: Category
      applies_to: [Icon]
      xml_fields: [Show]
      protobuf_field: trigger.action_show_category
      compatability: [TacO, BlishHUD, Burrito]

    - name: Hide Category
      description: Hide the specified category when triggered.
      type: Custom
      class: Category
      applies_to: [Icon]
      xml_fields: [Hide]
      protobuf_field: trigger.action_hide_category
      compatability: [TacO, BlishHUD, Burrito]

    - name: Toggle Category
      description: Show if hidden or Hide if shown the specified category when triggered.
      type: Custom
      class: Category
      applies_to: [Icon]
      xml_fields: [Toggle]
      protobuf_field: trigger.action_toggle_category
      compatability: [TacO, BlishHUD, Burrito]

    - name: Copy Message
      description: A value that is copied to the clipboard when triggered.
      type: String
      applies_to: [Icon]
      xml_fields: [CopyMessage]
      protobuf_field: trigger.action_copy_clipboard
      compatability: [TacO, BlishHUD, Burrito]

    - name: Copy Message
      description: A message that is displayed to the user when an item is copied to the clipboard.
      type: String
      applies_to: [Icon]
      xml_fields: [CopyMessage]
      protobuf_field: trigger.action_copy_message
      compatability: [TacO, BlishHUD, Burrito]

    - name: Invert Visibility
      description: Inverts the hide show behavior so that it is shown when it is supposed to be hidden and hidden when it is supposed to be shown.
      type: Boolean
      applies_to: [Icon]
      xml_fields: [InvertBehavior]
      protobuf_field: trigger.invert_display
      compatability: [TacO, BlishHUD, Burrito]

    - name: Bounce Height
      description: How high should the icon bounce when triggered.
      type: Float32
      applies_to: [Icon]
      xml_fields: [BounceHeight]
      protobuf_field: trigger.bounce_height
      compatability: [TacO, BlishHUD, Burrito]

    - name: Bounce Duration
      description: How long should the icon bounce for when triggered.
      type: Float32
      applies_to: [Icon]
      xml_fields: [BounceDuration]
      protobuf_field: trigger.bounce_duration
      compatability: [TacO, BlishHUD, Burrito]

    - name: Bounce Delay
      description: How many seconds should pass before the bouncing begins.
      type: Float32
      applies_to: [Icon]
      xml_fields: [BounceDelay]
      protobuf_field: trigger.bounce_delay
      compatability: [TacO, BlishHUD, Burrito]

    - name: Info Message
      description: Show a message to the user when triggered
      type: String
      applies_to: [Icon]
      xml_fields: [Info]
      protobuf_field: trigger.action_info_message
      compatability: [TacO, BlishHUD, Burrito]

    - name: Reset Behavior
      description: A behavior around triggers and retriggering.
      type: Enum
      applies_to: [Icon]
      xml_fields: [Behavior]
      protobuf_field: trigger.reset_behavior
      compatability: [TacO, BlishHUD, Burrito]
      values:
        always_visible: ["0", "always_visible"]
        map_change: ["1", "map_change"]
        daily_reset: ["2", "daily_reset"]
        never: ["3", "never"]
        timer: ["4", "timer"]
        map_reset: ["5", "map_reset"]
        instance_change: ["6", "instance_change"]
        daily_reset_per_character: ["7", "daily_reset_per_character"]

        weekly_reset: ["101", "weekly_reset"]

    - name: GUID
      description: A globally unique identifier value to make sure this maker's trigger reset data is always assocaited with this marker and never lost or confused with other markers.
      type: Custom
      class: UniqueID
      xml_fields: ["GUID"]
      applies_to: ["Icon", "Trail"]
      protobuf_field: "guid"
      compatability: [TacO, BlishHUD, Burrito]

    - name: Has Countdown
      description: "?"
      type: Boolean
      xml_fields: ["HasCountdown"]
      applies_to: ["Icon"]
      protobuf_field: trigger.has_countdown
      compatability: ["TacO"]
---

Notes
=====
https://blishhud.com/docs/markers/attributes/autotrigger
https://blishhud.com/docs/markers/attributes/triggerrange
https://blishhud.com/docs/markers/attributes/resetlength
https://blishhud.com/docs/markers/attributes/showhide
https://blishhud.com/docs/markers/attributes/toggle
https://blishhud.com/docs/markers/attributes/copy
https://blishhud.com/docs/markers/attributes/invertbehavior
https://blishhud.com/docs/markers/attributes/bounce
https://blishhud.com/docs/markers/attributes/info
https://blishhud.com/docs/markers/attributes/behavior
https://blishhud.com/docs/markers/attributes/guid





PARSEABLE_VAR(reset_offset, float, "ResetOffset") // Depricated

PARSEABLE_VAR(has_countdown, bool, "HasCountdown")