extends Sprite3D

const Utils = preload("utils.gd")

var texture_path
func set_icon_image(texture_path: String):
	self.texture_path = texture_path
	var texture = Utils.new().generate_texture(texture_path)
	self.texture = texture
	self.material_override.set_shader_param("texture_albedo", texture)
