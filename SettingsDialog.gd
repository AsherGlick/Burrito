extends WindowDialog

func load_settings():
	var minimum_width: LineEdit = $ScrollContainer/GridContainer/MinimumWidth
	minimum_width.text = String(Settings.minimum_width)
	var minimum_height: LineEdit = $ScrollContainer/GridContainer/MinimumHeight
	minimum_height.text = String(Settings.minimum_height)

	var override_size: CheckButton = $ScrollContainer/GridContainer/OverrideSize
	override_size.pressed = Settings.override_size_enabled
	var override_height: LineEdit = $ScrollContainer/GridContainer/OverrideHeight
	override_height.text = String(Settings.override_size_height)
	var override_width: LineEdit = $ScrollContainer/GridContainer/OverrideWidth
	override_width.text = String(Settings.override_size_width)

	var autolaunch_burrito_link: CheckButton = $ScrollContainer/GridContainer/AutoLaunchBurritoLink
	autolaunch_burrito_link.pressed = Settings.burrito_link_auto_launch_enabled
	var wine_path: LineEdit = $ScrollContainer/GridContainer/WinePath
	wine_path.text = Settings.burrito_link_wine_path
	var environment_vars: TextEdit = $ScrollContainer/GridContainer/EnvironmentVars
	environment_vars.text = Settings.burrito_link_env_args

func save_settings(new_value=null):
	var minimum_width: LineEdit = $ScrollContainer/GridContainer/MinimumWidth
	Settings.minimum_width = int(minimum_width.text)
	var minimum_height: LineEdit = $ScrollContainer/GridContainer/MinimumHeight
	Settings.minimum_height = int(minimum_height.text)

	var override_size: CheckButton = $ScrollContainer/GridContainer/OverrideSize
	Settings.override_size_enabled = override_size.pressed
	var override_height: LineEdit = $ScrollContainer/GridContainer/OverrideHeight
	Settings.override_size_height = int(override_height.text)
	var override_width: LineEdit = $ScrollContainer/GridContainer/OverrideWidth
	Settings.override_size_width = int(override_width.text)

	var autolaunch_burrito_link: CheckButton = $ScrollContainer/GridContainer/AutoLaunchBurritoLink
	Settings.burrito_link_auto_launch_enabled = autolaunch_burrito_link.pressed
	var wine_path: LineEdit = $ScrollContainer/GridContainer/WinePath
	Settings.burrito_link_wine_path = wine_path.text
	var environment_vars: TextEdit = $ScrollContainer/GridContainer/EnvironmentVars
	Settings.burrito_link_env_args = environment_vars.text

	Settings.save()

