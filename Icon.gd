extends Sprite3D

const Guildpoint = preload("res://guildpoint.gd")

var guildpoint: Guildpoint.Icon
var category: TreeItem

func set_icon_image(texture_path: String):
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

func set_position(position: Vector3):
	self.translation = position
