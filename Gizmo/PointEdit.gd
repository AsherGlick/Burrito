extends Spatial

var camera: Camera
signal selected(selected_object)
signal deselected()
signal updated(point_position)
var last_translation
var selected: bool = false


func clear_selection():
	self.selected = false
	$Node/CollisionShape.disabled = false
	$Plane.hide()
	$Plane/CollisionShape.disabled = true
	$Pillar.hide()
	$Pillar/CollisionShape.disabled = true
	emit_signal("deselected", self)


func select(camera, event):
	self.selected = true
	self.camera = camera
	$Node/CollisionShape.disabled = true
	$Plane.show()
	$Plane/CollisionShape.disabled = false
	$Pillar.show()
	$Pillar/CollisionShape.disabled = false
	emit_signal("selected", self)


################################################################################
# Handle resizing the control nodes so that no matter how far away from the
# camera they appear to be the same pixel size on-screen. Without this it will
# be harder to click on the arrows for things that are farther awawy.
################################################################################
func _process(delta):
	if (self.selected):
		var distance_to_camera = self.translation.distance_to(camera.translation + camera.get_parent().translation ) / 10
		var new_scale = Vector3(
			distance_to_camera,
			distance_to_camera,
			distance_to_camera
		)
		$Plane.scale = new_scale
		$Pillar.scale = new_scale

		if self.translation != self.last_translation:
			emit_signal("updated", self.translation)
			self.last_translation  = self.translation
