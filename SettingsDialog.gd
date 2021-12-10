extends WindowDialog

func load_settings():
	var override_size: CheckButton = $GridContainer/OverrideSize
	override_size.pressed = Settings.override_size_enabled
	var override_height: LineEdit = $GridContainer/OverrideHeight
	override_height.text = String(Settings.override_size_height)
	var override_width: LineEdit = $GridContainer/OverrideWidth
	override_width.text = String(Settings.override_size_width)

	var autolaunch_burrito_link: CheckButton = $GridContainer/AutoLaunchBurritoLink
	autolaunch_burrito_link.pressed = Settings.burrito_link_auto_launch_enabled
	var wine_path: LineEdit = $GridContainer/WinePath
	wine_path.text = Settings.burrito_link_wine_path
	var environment_vars: TextEdit = $GridContainer/EnvironmentVars
	environment_vars.text = Settings.burrito_link_env_args

func save_settings(new_value=null):
	var override_size: CheckButton = $GridContainer/OverrideSize
	Settings.override_size_enabled = override_size.pressed
	var override_height: LineEdit = $GridContainer/OverrideHeight
	Settings.override_size_height = int(override_height.text)
	var override_width: LineEdit = $GridContainer/OverrideWidth
	Settings.override_size_width = int(override_width.text)

	var autolaunch_burrito_link: CheckButton = $GridContainer/AutoLaunchBurritoLink
	Settings.burrito_link_auto_launch_enabled = autolaunch_burrito_link.pressed
	var wine_path: LineEdit = $GridContainer/WinePath
	Settings.burrito_link_wine_path = wine_path.text
	var environment_vars: TextEdit = $GridContainer/EnvironmentVars
	Settings.burrito_link_env_args = environment_vars.text

	Settings.save()

