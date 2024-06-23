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

static func generate_unique_file_name(base_name: String, extension: String, destination_folder: String) -> String:
	var unique_name: String = base_name
	var counter: int = 1
	var new_file_path: String = destination_folder.plus_file(unique_name + "." + extension)

	while File.new().file_exists(new_file_path):
		unique_name = base_name + "_" + str(counter)
		new_file_path = destination_folder.plus_file(unique_name + "." + extension)
		counter += 1
	return unique_name + "." + extension

static func copy_supported_image_file(file_path: String, destintation_dir: String) -> String:
	# These are all supported file types in Godot but may not be supported
	# by other marker programs.
	var supported_image_extensions: Array = ["png", "jpg", "jpeg", "bmp", "tga"]
	var file_name: String = file_path.get_file()
	var file_extension: String = file_name.get_extension().to_lower()
	if not supported_image_extensions.has(file_extension):
		print("File is not a supported image type. Please choose a png, jpg, jpeg, bmp, or tga.")
		return ""
	create_directory_if_missing(destintation_dir)
	var destination_path: String = destintation_dir.plus_file(file_name)

	var file: File = File.new()

	if file.file_exists(destination_path):
		file.open(file_path, File.READ)
		var original_size: int = file.get_len()
		var original_mod_time: int = file.get_modified_time(file_name)
		file.close()

		file.open(destination_path, File.READ)
		var destination_size: int = file.get_len()
		var destination_mod_time: int = file.get_modified_time(file_name)
		file.close()

		if original_size == destination_size and original_mod_time == destination_mod_time:
			return destination_path
		else:
			var base_name: String = file_name.get_basename()
			var extension: String = file_name.get_extension()
			var unique_name: String = generate_unique_file_name(base_name, extension, destintation_dir)
			destination_path = destintation_dir.plus_file(unique_name)

	file.open(file_path, File.READ)
	var content: PoolByteArray = file.get_buffer(file.get_len())
	file.close()

	var result: int = file.open(destination_path, File.WRITE)
	if result == OK:
		file.store_buffer(content)
		file.close()
		print("File imported successfully.")
		return destination_path
	else:
		print("Failed to import file.")
		return ""
