extends FileDialog

const FileHandler = preload("res://FileHandler.gd")
var downloaded_markers_dir: String
var unsaved_markers_dir: String
var user_data_dir: String

func _ready():
	#TODO: Move these to global file Settings so they can be customized
	self.user_data_dir = str(OS.get_user_data_dir())
	self.downloaded_markers_dir = self.user_data_dir.plus_file("packs")
	self.unsaved_markers_dir = self.user_data_dir.plus_file("protobins")
	FileHandler.create_directory_if_missing(self.downloaded_markers_dir)
	FileHandler.create_directory_if_missing(self.unsaved_markers_dir)

func _on_FileDialog_dir_selected(dir_path: String):
	print("Selected folder:", dir_path)
	var new_path: String = self.downloaded_markers_dir.plus_file(dir_path.get_file())
	FileHandler.create_directory_if_missing(new_path)
	var args: PoolStringArray = [
		"--input-taco-path", dir_path,
		"--output-guildpoint-path", new_path,
		"--output-split-guildpoint-path", self.unsaved_markers_dir
	]
	FileHandler.call_burrito_converter(args)
