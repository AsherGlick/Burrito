extends FileDialog
const FileHandler = preload("res://FileHandler.gd")
var file_handler: FileHandler

func _ready():
	self.file_handler = FileHandler.new()
	pass

func _on_FileDialog_dir_selected(dir_path):
	print("Selected folder:", dir_path)
	var new_path: String = file_handler.protobin_data_folder.plus_file(dir_path.get_file())
	file_handler.ensure_folder_existance(new_path)
	#else: 
	#	#Pop up here to confirm overwrite?
	var args: PoolStringArray = [
		"--input-taco-path", dir_path,
		"--output-waypoint-path", new_path,
		"--output-split-waypoint-path", file_handler.split_protobin_data_folder
	]
	file_handler.call_xml_converter(args)
