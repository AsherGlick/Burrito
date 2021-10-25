class_name Utils

func load_image_from_path(path: String):
	var file = File.new()
	if !file.file_exists(path):
		print("Warning: File does not exist: ", path)
	var image = Image.new()
	file.open(path, File.READ)
	image.load_png_from_buffer(file.get_buffer(file.get_len()))
	file.close()
	image.lock()
	return image

func generate_texture(texture_path: String, color_space: String):
	var image = load_image_from_path(texture_path)
	var texture = ImageTexture.new()
	var flag = 7
	var accepted_color_spaces = ['sRGB', 'RGB']
	if color_space == 'sRGB':
		flag = 22
	if !accepted_color_spaces.has(color_space):
		print("Only RGB and sRGB accepted. Defaulting to RGB.")
	texture.create_from_image(image, flag)
	texture.storage = ImageTexture.STORAGE_COMPRESS_LOSSLESS
	return texture