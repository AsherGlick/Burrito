extends Control

const executable_path: String = "./xml_converter/build/xml_converter"
var protobin_data_folder: String
var split_protobin_data_folder: String
var user_data_dir: String 

func _ready():
	var dir = Directory.new()
	self.user_data_dir = str(OS.get_user_data_dir())
	self.protobin_data_folder = self.user_data_dir.plus_file("packs")
	self.split_protobin_data_folder = self.user_data_dir.plus_file("protobins")
	if not dir.dir_exists(self.protobin_data_folder):
		var success = dir.make_dir(self.protobin_data_folder)
		if success != OK:
			print("Error: Could not create data folder:", self.protobin_data_folder)
	if not dir.dir_exists(self.split_protobin_data_folder):
		var success = dir.make_dir(self.split_protobin_data_folder)
		if success != OK:
			print("Error: Could not create data folder:", self.split_protobin_data_folder)

func _on_FileDialog_dir_selected(dir_path):
	var output: Array = []
	print("Selected folder:", dir_path)
	var dir = Directory.new()
	var new_path: String = self.protobin_data_folder.plus_file(dir_path.get_file())
	if not dir.dir_exists(new_path):
		var success = dir.make_dir(new_path)
		if success != OK:
			print("Error: Could not create data folder:", self.protobin_data_folder)
	#else: 
	#	#Pop up here to confirm overwrite?
	var args: PoolStringArray = [
		"--input-taco-path", dir_path,
		"--output-waypoint-path", new_path,
	]
	print(args)
	var result: int = OS.execute(self.executable_path, args, true, output, true)
	print(output)
	if result != OK:
		print("Failed to execute the command. Error code:", result)
	else:
		print("Command executed successfully.")
	split_waypoint_markers()
	get_node("../../..").load_waypoint_markers()

func split_waypoint_markers():
	var input_waypoint_paths: Array = []
	var output: Array = []
	var dir = Directory.new()
	if dir.open(self.protobin_data_folder) == OK:
		dir.list_dir_begin(true)
		var file_name = dir.get_next()
		while file_name != "":
			if dir.current_is_dir():
				input_waypoint_paths.append(self.protobin_data_folder.plus_file(file_name.plus_file("markers.bin")))
			file_name = dir.get_next()
			print(file_name)
	else:
		print("An error occurred when trying to access ", self.protobin_data_folder)
		return
	var args: PoolStringArray = ["--input-waypoint-path"]
	args.append_array(input_waypoint_paths)
	args.append_array([
		"--output-split-waypoint-path", self.split_protobin_data_folder,
		])
	print(args)
	var result: int = OS.execute(self.executable_path, args, true, output, true)
	print(output)
	if result != OK:
		print("Failed to execute the command. Error code:", result)
	else:
		print("Command executed successfully.")
