extends Sprite3D

var texture_path
var category_name = ""
var is_editable = false

func set_icon_image(texture_path: String):
	self.texture_path = texture_path
	
	var texture_file = File.new()
	var image = Image.new()
	texture_file.open(texture_path, File.READ)
	image.load_png_from_buffer(texture_file.get_buffer(texture_file.get_len()))
	texture_file.close()
	image.lock()

	var texture = ImageTexture.new()
	texture.create_from_image(image) #, 6)
	
	
	self.texture = texture
	self.material_override.set_shader_param("texture_albedo", texture)
