extends Sprite3D

const Waypoint = preload("res://waypoint.gd")

var texture_path
var waypoint: Waypoint.Icon
var category: TreeItem

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

func update_point_poistion(position: Vector3):
	self.translation = position

func remove_point(index):
	queue_free()
