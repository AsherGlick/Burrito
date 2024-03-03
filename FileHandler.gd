extends Control

const executable_path: String = "./xml_converter/build/xml_converter"
var protobin_data_folder: String
var split_protobin_data_folder: String
var user_data_dir: String 

func _ready():
	self.user_data_dir = str(OS.get_user_data_dir())
	self.protobin_data_folder = self.user_data_dir.plus_file("packs")
	self.split_protobin_data_folder = self.user_data_dir.plus_file("protobins")
	ensure_folder_existance(self.protobin_data_folder)
	ensure_folder_existance(self.split_protobin_data_folder)

func call_xml_converter(args):
	var output: Array = []
	print(args)
	var result: int = OS.execute(self.executable_path, args, true, output, true)
	print(output)
	if result != OK:
		print("Failed to execute the command. Error code:", result)
	else:
		print("Command executed successfully.")

func ensure_folder_existance(path):
	var dir = Directory.new()
	if not dir.dir_exists(path):
		var success = dir.make_dir(path)
		if success != OK:
			print("Error: Could not create data folder:", path)
