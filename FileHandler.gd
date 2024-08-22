const converter_executable_path = "./xml_converter/build/xml_converter"

static func call_xml_converter(args: PoolStringArray):
	var output: Array = []
	print(args)
	var result: int = OS.execute(converter_executable_path, args, true, output, true)
	print(output)
	if result != OK:
		print("Failed to execute the converter. Error code:", result)
	else:
		print("Command executed successfully.")

static func create_directory_if_missing(path: String):
	var dir = Directory.new()
	if not dir.dir_exists(path):
		var success = dir.make_dir(path)
		if success != OK:
			print("Error: Could not create data folder:", path)

static func find_file_duplicate(directory_path: String, target_name: String, target_content: PoolByteArray, relative_path: String):
	var dir = Directory.new()
	if dir.open(directory_path) != OK:
		return null
	dir.list_dir_begin(true)
	var file_name = dir.get_next()
	while file_name != "":
		var full_path = directory_path.plus_file(file_name)
		var current_relative_path = relative_path.plus_file(file_name)
		if dir.current_is_dir():
			var found_path = find_file_duplicate(full_path, target_name, target_content, current_relative_path)
			if found_path != "":
				dir.list_dir_end()
				return found_path
		if file_name == target_name:
			var file = File.new()
			file.open(full_path, File.READ)
			var file_content = file.get_buffer(file.get_len())
			file.close()
			if file_content == target_content:
				dir.list_dir_end()
				return current_relative_path
		file_name = dir.get_next()
	return null

static func find_image_duplicates(file_path: String, destintation_dir: String):
	var file_name: String = file_path.get_file()
	var file_extension: String = file_name.get_extension().to_lower()
	if not file_extension == "png":
		print("File is not a supported image type. Please choose a png")
		return ""

	var file: File = File.new()
	file.open(file_path, File.READ)
	var file_content = file.get_buffer(file.get_len())
	var relative_path = find_file_duplicate(destintation_dir, file_name, file_content, "")
	return relative_path


static func copy_file(file_path: String, destination_path: String):
	var dir = Directory.new()
	var result = dir.copy(file_path, destination_path)
	if result == OK:
		print("File imported successfully.")
	else:
		print("Failed to import file.")
