var downloaded_markers_dir: String
var unsaved_markers_dir: String
var user_data_dir: String 

func _init():
	self.user_data_dir = str(OS.get_user_data_dir())
	self.downloaded_markers_dir = self.user_data_dir.plus_file("packs")
	self.unsaved_markers_dir = self.user_data_dir.plus_file("protobins")
	create_directory_if_missing(self.downloaded_markers_dir)
	create_directory_if_missing(self.unsaved_markers_dir)

static func call_xml_converter(args):
	var executable_path: String = "./xml_converter/build/xml_converter"
	var output: Array = []
	print(args)
	var result: int = OS.execute(executable_path, args, true, output, true)
	print(output)
	if result != OK:
		print("Failed to execute the command. Error code:", result)
	else:
		print("Command executed successfully.")

static func create_directory_if_missing(path):
	var dir = Directory.new()
	if not dir.dir_exists(path):
		var success = dir.make_dir(path)
		if success != OK:
			print("Error: Could not create data folder:", path)
