extends FileDialog
const FileHandler = preload("res://FileHandler.gd")
var downloaded_markers_dir: String
var unsaved_markers_dir: String
var user_data_dir: String

func _on_FileDialog_dir_selected(dir_path):
	print("Selected folder:", dir_path)
	self.user_data_dir = str(OS.get_user_data_dir())
	self.downloaded_markers_dir = self.user_data_dir.plus_file("packs")
	self.unsaved_markers_dir = self.user_data_dir.plus_file("protobins")
	FileHandler.create_directory_if_missing(self.downloaded_markers_dir)
	FileHandler.create_directory_if_missing(self.unsaved_markers_dir)
	var new_path: String = self.downloaded_markers_dir.plus_file(dir_path.get_file())
	FileHandler.create_directory_if_missing(new_path)
	#else: 
	#	#Pop up here to confirm overwrite?
	var args: PoolStringArray = [
		"--input-taco-path", dir_path,
		"--output-waypoint-path", new_path,
		"--output-split-waypoint-path", self.unsaved_markers_dir
	]
	FileHandler.call_xml_converter(args)
