extends "res://Gizmo/PlaneDragmove.gd"

var selected:bool = false;
var start_position: Vector3
var start_offset: Vector3

################################################################################
# When selected create a plane that is perpendicular to the camera in the xz
# space and perfectly vertical in the y space. We will then use the y-value
# of the intersection between the mouse and this plane to determine the y-value
# of the gizmo.
################################################################################
func select(camera: Camera, event: InputEvent):
	self.selected = true
	self.start_position = self.get_parent().translation
	self.camera = camera

	var node_pos: Vector2 = Vector2(self.get_parent().translation.x, self.get_parent().translation.z)
	var plane_range: Vector2 = Vector2(camera.global_transform.basis.z.x, camera.global_transform.basis.z.z)

	var k = plane_range.x * node_pos.x + plane_range.y * node_pos.y

	# In the planar format of [0]x + [1]y +[2]z = [3]
	self.plane = [plane_range.x, 0, plane_range.y, k]

	self.start_offset = get_offset_coordinates(event)


################################################################################
#
################################################################################
func _input(event:InputEvent):
	if selected:
		if event is InputEventMouseButton:
			if not event.pressed and event.button_index == 1:
				selected = false
				_show_hover()
		elif event is InputEventMouseMotion:
			var new_offset = get_offset_coordinates(event)

			var delta_offset = new_offset - self.start_offset
			# Lock Changes to only the y axis
			delta_offset.x = 0
			delta_offset.z = 0

			self.get_parent().translation = self.start_position + delta_offset

var hovered = false
func hover():
	self.hovered = true
	_show_hover()
func unhover():
	self.hovered = false;
	if !self.selected:
		_show_hover()

func _show_hover():
	$ArrowMesh.get_surface_material(0).set_shader_param("hovering", self.hovered)
