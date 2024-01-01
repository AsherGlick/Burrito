extends Control

const executable_path: String = "./xml_converter/build/xml_converter"
var protobin_data_folder: String
var split_protobin_data_folder: String
var user_data_dir: String 
onready var output_dialog = $OutputDialog
onready var output_label = $OutputDialog/ScrollContainer/OutputLabel

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
		"--output-split-waypoint-path", self.split_protobin_data_folder
	]
	custom_print(args)
	var result: int = OS.execute(self.executable_path, args, true, output)
	custom_print(output)
	if result != OK:
		print("Failed to execute the command. Error code:", result)
	else:
		print("Command executed successfully.")

func custom_print(message: Array):
	output_dialog.show()
	for line in message:
		output_label.text += line + "\n"
