extends Control

const executable_path: String = "./xml_converter/build/xml_converter"
var protobin_data_folder: String
var split_protobin_data_folder: String
var user_data_dir: String 
onready var output_dialog = $WindowDialog
onready var output_label = $WindowDialog/ScrollContainer/Label

func _ready():
	var dir = Directory.new()
	self.user_data_dir = str(OS.get_user_data_dir())
	self.protobin_data_folder = self.user_data_dir.plus_file("packs")
	self.split_protobin_data_folder = self.user_data_dir.plus_file("protobins")
	if not dir.dir_exists(self.protobin_data_folder):
		var success = dir.make_dir(self.protobin_data_folder)
		if success != OK:
			print("Error: Could not create data folder:", self.protobin_data_folder)


func clear_directory_contents(dir_path: String) -> void:
	var dir = Directory.new()
	if dir.open(dir_path) == OK:
		while dir.get_next():
			var entry_path: String = dir_path + "/" + dir.get_file()
			# Remove subdirectories and files, but not "." and ".."
			if dir.current_is_dir() and dir.get_file() != "." and dir.get_file() != "..":
				clear_directory_contents(entry_path)
				# Remove the subdirectory after its contents are cleared
				dir.remove(entry_path)
			elif dir.current_is_file():
				dir.remove(entry_path)
	else:
		print("Failed to open directory:", dir_path)

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
	#	clear_directory_contents(dir_path)
	var args: PoolStringArray = [
		"--input-taco-path", dir_path,
		"--output-waypoint-path", new_path,
		"--output-split-waypoint-path", self.split_protobin_data_folder]
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

	
