class_name Utils

func load_image_from_path(path: String):
	var file = File.new()
	var image = Image.new()
	file.open(path, File.READ)
	image.load_png_from_buffer(file.get_buffer(file.get_len()))
	file.close()
	image.lock()
	return image

func generate_texture(texture_path: String):
	var image = load_image_from_path(texture_path)
	var texture = ImageTexture.new()
	texture.create_from_image(image)
	return texture