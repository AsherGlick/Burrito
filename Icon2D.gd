extends Sprite

const Utils = preload("utils.gd")

func set_icon_image(texture_path: String):
	var texture = Utils.new().generate_texture(texture_path, 'RGB')
	self.texture = texture
	# TODO: Figure out a better heuristic. Why 0.05?
	var scaling_factor = 0.05
	self.apply_scale(Vector2(scaling_factor, scaling_factor))
