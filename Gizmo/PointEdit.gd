extends Spatial

var camera: Camera
signal selected(selected_object)
signal deselected(selected_object)

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

var object_link = null
var object_2d_link = null
var object_index:int = 0
var point_type: String

func link_point(point_type: String, object_link, object_2d_link = null, object_index = 0):
	self.point_type = point_type
	self.object_link = object_link
	if point_type == "path" || point_type == "area":
		self.object_2d_link = object_2d_link
		self.object_index = object_index
	if point_type == "icon":
		pass

func update_point():
	if point_type == "path" || point_type == "area":
		# Regeneration of 3d paths can take a while and will cause lag if we 
		# try to update it every frame. Instead we only update it if the mouse
		# is not pressed (dragging a node) and if the position is different
		# from what is used to be. There may be a better way to generate paths
		# in the future that is less resource intensive but the original
		# solution was chosen for its implementation speed instead of its
		# generation speed.
		if (
			self.object_link.curve.get_point_position(self.object_index) != self.translation
			and not Input.is_mouse_button_pressed( 1 )
		):
			self.object_link.curve.set_point_position(self.object_index, self.translation)
		
		self.object_2d_link.points[self.object_index] = Vector2(self.translation.x, self.translation.z)
	if point_type == "icon":
		self.object_link.translation = self.translation


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
		
		update_point()
