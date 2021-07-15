extends "res://Gizmo/PlaneDragmove.gd"

var selected:bool = false;
var start_position: Vector3
var start_offset: Vector3


func select(camera, event):
	self.selected = true
	self.start_position = self.get_parent().translation
	self.camera = camera

	# Define a flat plane at the height of the current y value
	self.plane = [0, 1, 0, self.get_parent().translation.y]
	self.start_offset = get_offset_coordinates(event)


func _input(event:InputEvent):
	if selected:
		if event is InputEventMouseButton:
			if not event.pressed and event.button_index == 1:
				selected = false
				_show_hover()
		elif event is InputEventMouseMotion:
			var new_offset = get_offset_coordinates(event)
			var delta_offset = new_offset - self.start_offset

			# Catch if the new position is behind the camera, if so flip it
			# because it is probably a horizon cross
			var angle_diff = (camera.global_transform.origin - (self.start_position + delta_offset)).dot(camera.global_transform.basis.z)
			if (angle_diff < 0):
				delta_offset = -delta_offset + (self.start_position - camera.global_transform.origin)

			# Lock off y axis movement so this only is xz
			delta_offset.y = 0

			# This should be a square instead of a circle
			if delta_offset.length_squared() > 10000:
				delta_offset = delta_offset * 100/delta_offset.length()

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
	$ArrowMesh2.get_surface_material(0).set_shader_param("hovering", self.hovered)
