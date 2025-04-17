extends Control

func _ready():
	Settings.connect("settings_updated", self, "_update_global_menu_button_position")
	_update_global_menu_button_position()

func _update_global_menu_button_position():
	if Settings.override_burrito_icon_position_enabled == true:
		self.set_position(Vector2(Settings.override_burrito_icon_horizontal_position, Settings.override_burrito_icon_vertical_position))

