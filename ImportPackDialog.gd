extends FileDialog
const FileHandler = preload("res://FileHandler.gd")

func _on_FileDialog_dir_selected(dir_path):
	print("Selected folder:", dir_path)
	var new_path: String = FileHandler.downloaded_markers_dir.plus_file(dir_path.get_file())
	FileHandler.create_directory_if_missing(new_path)
	#else: 
	#	#Pop up here to confirm overwrite?
	var args: PoolStringArray = [
		"--input-taco-path", dir_path,
		"--output-waypoint-path", new_path,
		"--output-split-waypoint-path", FileHandler.unsaved_markers_dir
	]
	FileHandler.call_xml_converter(args)
