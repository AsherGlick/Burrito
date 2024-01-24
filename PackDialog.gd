extends Control

const executable_path: String = "./xml_converter/build/xml_converter"
var user_marker_pack_dir: String
var user_protobin_by_map_id_dir: String
var user_data_dir: String 

func _ready():
	var dir = Directory.new()
	self.user_data_dir = str(OS.get_user_data_dir())
	self.user_marker_pack_dir = self.user_data_dir.plus_file("marker_packs")
	self.user_protobin_by_map_id_dir = self.user_data_dir.plus_file("protobin_by_map_id")
	if not dir.dir_exists(self.user_marker_pack_dir):
		var success = dir.make_dir(self.user_marker_pack_dir)
		if success != OK:
			print("Error: Could not create data folder:", self.user_marker_pack_dir)
	if not dir.dir_exists(self.user_protobin_by_map_id_dir):
		var success = dir.make_dir(self.user_protobin_by_map_id_dir)
		if success != OK:
			print("Error: Could not create data folder:", self.user_protobin_by_map_id_dir)

func _on_FileDialog_dir_selected(dir_path):
	var output: Array = []
	print("Selected folder:", dir_path)
	var dir = Directory.new()
	var new_path: String = self.user_marker_pack_dir.plus_file(dir_path.get_file())
	if not dir.dir_exists(new_path):
		var success = dir.make_dir(new_path)
		if success != OK:
			print("Error: Could not create data folder:", self.user_marker_pack_dir)
	#else: 
	#	#Pop up here to confirm overwrite?
	var args: PoolStringArray = [
		"--input-taco-path", dir_path,
		"--output-waypoint-path", new_path,
		"--output-split-waypoint-path", self.user_protobin_by_map_id_dir
	]
	print(args)
	var result: int = OS.execute(self.executable_path, args, true, output, true)
	print(output)
	if result != OK:
		print("Failed to execute the command. Error code:", result)
	else:
		print("Command executed successfully.")
