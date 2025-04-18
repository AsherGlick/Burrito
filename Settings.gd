extends Node

const CONFIG_PATH = "user://settings.json"
const FileHandler = preload("res://FileHandler.gd")

var _config_data = {}
var local_category_data = {}

var minimum_width: int = 800
var minimum_height: int = 600

var override_size_enabled: bool = false;
var override_size_height: int = 1080
var override_size_width: int = 1920

var burrito_link_auto_launch_enabled: bool = false
var burrito_link_wine_path: String = ""
var burrito_link_env_args: String = ""

# We save the marker data in this directory when the files are have been split
# by Map ID. All changes made by the editor are automatically saved in these
# files prior to export.
var unsaved_markers_dir = "user://protobin_by_map_id/"
var saved_markers_dir = "user://protobin/"

func _ready():
	var file = File.new()
	file.open(CONFIG_PATH, file.READ)
	var text = file.get_as_text()
	var datum = JSON.parse(text)
	self._config_data = JSON.parse(text).result

	if self._config_data == null:
		self._config_data = {}
		
	if "local_category_data" in self._config_data:
		self.local_category_data = self._config_data["local_category_data"]
	if "minimum_width" in self._config_data:
		self.minimum_width = self._config_data["minimum_width"]
	if "minimum_height" in self._config_data:
		self.minimum_height = self._config_data["minimum_height"]
	if "override_size_enabled" in self._config_data:
		self.override_size_enabled = self._config_data["override_size_enabled"]
	if "override_size_height" in self._config_data:
		self.override_size_height = self._config_data["override_size_height"]
	if "override_size_width" in self._config_data:
		self.override_size_width = self._config_data["override_size_width"]
	if "burrito_link_auto_launch_enabled" in self._config_data:
		self.burrito_link_auto_launch_enabled = self._config_data["burrito_link_auto_launch_enabled"]
	if "burrito_link_wine_path" in self._config_data:
		self.burrito_link_wine_path = self._config_data["burrito_link_wine_path"]
	if "burrito_link_env_args" in self._config_data:
		self.burrito_link_env_args = self._config_data["burrito_link_env_args"]

func get_saved_markers_dir() -> String:
	FileHandler.create_directory_if_missing(self.saved_markers_dir)
	return self.saved_markers_dir

func get_unsaved_markers_dir() -> String:
	FileHandler.create_directory_if_missing(self.unsaved_markers_dir)
	return self.unsaved_markers_dir

func save():
	_config_data = {
		"minimum_width": minimum_width,
		"minimum_height": minimum_height,
		"override_size_enabled": override_size_enabled,
		"override_size_height": override_size_height,
		"override_size_width": override_size_width,
		"burrito_link_auto_launch_enabled": burrito_link_auto_launch_enabled,
		"burrito_link_wine_path": burrito_link_wine_path,
		"burrito_link_env_args": burrito_link_env_args,
		"local_category_data": local_category_data
	}

	var file = File.new()
	file.open(CONFIG_PATH, File.WRITE)
	file.store_string(JSON.print(self._config_data, "    "))
