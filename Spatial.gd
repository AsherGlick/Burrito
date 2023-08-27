extends Spatial

var server := UDPServer.new()
var peers = []

var map_id:int = 0

var Waypoint = preload("res://waypoint.gd")
var map_is_open: bool
var compass_is_top_right: bool

var edit_panel_open: bool = false

# This is the path to the texture that will be used for the next created 3d-path
# object or icon object in the UI
var next_texture_path: String = ""

# A pointer to the path object that any new nodes should be appended to. If
# the value is null then a new path will be created the next time a path point
# will be created.
var currently_active_path = null
var currently_active_path_2d = null
var currently_active_category = null

var map_was_open = false

# Player Position as accurate to Guild Wars 2
var player_position := Vector3(0,0,0)

# Player Position as accurate to Godot (z value sign is flipped)
var correct_player_position := Vector3(0,0,0) 

var compass_height: int = 0;
var compass_width: int = 0;


# A temporary setting able to be configured by the user. It is used to allow
# for faster trail mesh generation. The higher the value the fewer samples are
# taken for the MeshCSG leading to an overall lower number of polygons. 
var path_resolution = 1

# Variables that store opposit corners of the compass
var compass_corner1
var compass_corner2
#x11 fg and window id
var x11_fg: X11_FG
var taco_parser: TacoParser
var x11_window_id_burrito: int
var is_transient:bool = false

# Called when the node enters the scene tree for the first time.
func _ready():
	get_tree().get_root().set_transparent_background(true)
	x11_fg = X11_FG.new()
	taco_parser = TacoParser.new()
	x11_window_id_burrito = OS.get_native_handle(OS.WINDOW_HANDLE)
	OS.window_maximized = false
	# Start off with a small size before GW2 client is up
	OS.window_size = Vector2(800, 600)
	# Postion at top left corner
	OS.set_window_position(Vector2(0,0))
	set_minimal_mouse_block()
	init_category_tree()
	marker_file_dir.open("user://protobins/")
	server.listen(4242)

func set_minimal_mouse_block():
	var top_corner := Vector2(287, 0)
	var bottom_corner := Vector2(314, 32)
	
	if self.edit_panel_open:
		bottom_corner.y = 49
		bottom_corner.x = 314+377
	
	var clickthrough: PoolVector2Array = [
		Vector2(top_corner.x ,top_corner.y),
		Vector2(bottom_corner.x, top_corner.y),
		Vector2(bottom_corner.x,bottom_corner.y),
		Vector2(top_corner.x ,bottom_corner.y)
	]
	OS.set_window_mouse_passthrough(clickthrough)
	$Control/Border.hide()

func set_maximal_mouse_block():
	OS.set_window_mouse_passthrough([])
	$Control/Border.show()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	#OS.window_position = Vector2(1920, 0) # TODO: This does not seem to work	
	#OS.set_window_position(Vector2(1920,0))
	#print(OS.window_position)
	server.poll() # Important
	if server.is_connection_available():
		var peer: PacketPeerUDP = server.take_connection()
		print("accepted peer: %s:%s" % [peer.get_packet_ip(), peer.get_packet_port()])
		peers.append(peer)

	for i in range(0, peers.size()):
		var peer: PacketPeerUDP = peers[i]
		for j in range(0, peer.get_available_packet_count()):

			var spb := StreamPeerBuffer.new()
			spb.data_array = peer.get_packet()

			var packet_type:int = spb.get_8()

			if packet_type == 1:
				decode_frame_packet(spb)
			elif packet_type == 2:
				decode_context_packet(spb)	
	return


func decode_frame_packet(spb: StreamPeerBuffer):
	# Extract the position of the camera
	var camera_position = Vector3(
		spb.get_float(),
		spb.get_float(),
		spb.get_float()
	)
	
	# Extract the rotation of the camera in the form of a normal vector
	var camera_facing = Vector3(
		spb.get_float(),
		spb.get_float(),
		spb.get_float()
	)
	
	# Extract the position of the player's foot
	self.player_position = Vector3(
		spb.get_float(),
		spb.get_float(),
		spb.get_float()
	)
	self.correct_player_position = Vector3(player_position.x, player_position.y, -player_position.z)
	
	if $Control/Position.visible:
		$Control/Position.text = "X " + str(player_position.x) + "   Y " + str(player_position.y) + "   Z " + str(-player_position.z)
	
	var map_offset = Vector2(
		spb.get_float(),
		spb.get_float()
	)
	var map_scale: float = spb.get_float()
	if map_scale == 0: #fixing a divide by zero error that occurs when the game loads
		map_scale = 0.000001
	var map_rotation: float = spb.get_float()
	var ui_flags: int = spb.get_32()
	
	map_is_open = (ui_flags & 0x01) == 0x01;
	compass_is_top_right = (ui_flags & 0x02) == 0x02;
	var compass_rotation_is_enabled: bool = (ui_flags & 0x04) == 0x04;
	var game_is_focused: bool = (ui_flags & 0x08) == 0x08;
	var in_competitive_game_mode: bool = (ui_flags & 0x10) == 0x10;
	var chatbox_has_focus: bool = (ui_flags & 0x20) == 0x20;
	var in_combat: bool = (ui_flags & 0x40) == 0x40;
	var unchecked_flag = (ui_flags & 0xFFFFFF80) != 0x00000000;

	if map_is_open != map_was_open:
		$Paths.visible = !map_is_open
		$Icons.visible = !map_is_open
		reset_minimap_masks()
		map_was_open = map_is_open

	if unchecked_flag:
		print("Unchecked Flag", (ui_flags & 0xFFFFFF80))

	# Position the camera in the same location as it in in-game
	$CameraMount.translation.x = camera_position.x
	$CameraMount.translation.y = camera_position.y
	$CameraMount.translation.z = -camera_position.z
	
	# Orent the camera in the same rotation as it is facing in game
	$CameraMount/Camera.rotation.x = asin(camera_facing.y)
	$CameraMount.rotation.y = -atan2(camera_facing.x, camera_facing.z)

	# in-engine units are meters
	# in-game units are inches
	# continent map units are 24inches to 1 condinenet map unit
	# 1 meter = 39.37008 inches
	# 39.37008 / 24 = 1.64042
	# meters -> 24 inches
	# Info gathered from https://wiki.guildwars2.com/wiki/API:Maps
	var map_object_scaling = 1.64042/map_scale

	var map_size = get_viewport().size
	var map_corner = Vector2(0, 0)
	
	if (!map_is_open):
		map_size = Vector2(compass_width, compass_height)
		if !compass_is_top_right:
			map_corner = get_viewport().size - Vector2(compass_width, compass_height + 36)
		else:
			map_corner = Vector2(get_viewport().size.x - compass_width, 0)

	var player_map_position: Vector2 = Vector2(player_position.x, -player_position.z)*map_object_scaling

	var delta_position = Vector2(0,0)
	if (map_rotation != 0 && !map_is_open):
		var pivot = Vector2(0,0)
		var radians = map_rotation

		var cosTheta = cos(radians)
		var sinTheta = sin(radians)

		var x = (cosTheta * (player_map_position.x - pivot.x) - sinTheta * (player_map_position.y - pivot.y) + pivot.x);
		var y = (sinTheta * (player_map_position.x - pivot.x) + cosTheta * (player_map_position.y - pivot.y) + pivot.y);
		
		delta_position = player_map_position - Vector2(x, y);
		
		#print(map_rotation)
		$Control/MiniMap.rotation = map_rotation
	else:
		$Control/MiniMap.rotation = 0
	
	var map_midpoint = map_size/2 + map_corner;
	
	$Control/MiniMap.scale=Vector2(map_object_scaling, map_object_scaling)
	var map_translation = map_offset
	$Control/MiniMap.position = (map_translation / map_scale) + map_midpoint - player_map_position + delta_position
	var new_feet_location = Vector3(player_position.x, player_position.y, -player_position.z)
	$FeetLocation.translation = new_feet_location


func decode_context_packet(spb: StreamPeerBuffer):
	compass_width = spb.get_16()
	compass_height = spb.get_16()
	var old_map_id = self.map_id
	self.map_id  = spb.get_32()

	var x11_window_id_gw2 = spb.get_32()
	if !is_transient:
		is_transient = x11_fg.set_transient_for(x11_window_id_burrito, x11_window_id_gw2)

	var size = Vector2(800, 600)
	if Settings.override_size_enabled:
		size.x = Settings.override_size_width
		size.y = Settings.override_size_height
	else:
		var size_tuple = x11_fg.get_window_geometry(x11_window_id_gw2)
		size.x = size_tuple[0]
		size.y = size_tuple[1]
	OS.window_size = size
	var identity_length: int = spb.get_32()
	var identity_str = spb.get_utf8_string(identity_length)
	var identity = JSON.parse(identity_str).result
	
	# FOV Calculations
	# The minimum value on the FOV slider gives a float value in this field of 0.436
	# The maximum value on the FOV slider gives a float value in this field of 1.222
	# According to https://thatshaman.com/tools/fov/ the FOV range is (25 -> 70)
	# This function maps a value between 0.436 and 1.222 to a value between 25 and 70
	$CameraMount/Camera.fov = (((identity["fov"] - 0.436) / 0.786) * 45) + 25
	# TODO: after looking back at this it has become obvious this is just degrees
	# vs radians. 70deg = 1.22173rad and 25deg = 0.4363323rad. We should redo
	# this to just be a radian to degree conversion.

	if self.map_id != old_map_id:
		print("Loading New Map")
		load_waypoint_markers(self.map_id)

	# TODO move this to reset_minimap_masks
	for child in $Paths.get_children():
		child.get_node("MeshInstance").material_override.set_shader_param("map_size", Vector2(compass_width, compass_height))

	# TODO move this to reset_minimap_masks
	for icon in $Icons.get_children():
		icon.material_override.set_shader_param("map_size", Vector2(compass_width, compass_height))

	reset_minimap_masks()

func reset_minimap_masks():
	var viewport_size = get_viewport().size
	compass_corner1 = Vector2(0, 0)
	compass_corner2 = viewport_size
	if !map_is_open && !compass_is_top_right:
		compass_corner1 = Vector2(viewport_size.x-compass_width, 36)
		compass_corner2 = compass_corner1 + Vector2(compass_width, compass_height)
	elif !map_is_open && compass_is_top_right:
		compass_corner1 = viewport_size - Vector2(compass_width, compass_height)
		compass_corner2 = compass_corner1 + Vector2(compass_width, compass_height)
	
	for minimap_path in $Control/MiniMap.get_children():
		minimap_path.material.set_shader_param("minimap_corner", compass_corner1)
		minimap_path.material.set_shader_param("minimap_corner2", compass_corner2)

var Waypoint_data = Waypoint.Waypoint.new()
var marker_file_dir = Directory.new()
var marker_file_path = ""
var root: TreeItem

##########Node Connections###########
onready var marker_packs = $Control/Dialogs/MarkerPacks/MarkerPacks
onready var icons = $Icons
onready var paths = $Paths
onready var minimap = $Control/MiniMap


func load_waypoint_markers(map_id):
	self.marker_file_path = self.marker_file_dir.get_current_dir() + String(map_id) + ".data"
	self.Waypoint_data.clear_category()
	clear_map_markers()
	root.free()
	init_category_tree()
	var file = File.new()
	print("Loading protobuf file from path ", self.marker_file_path)
	file.open(self.marker_file_path, file.READ)
	var data = file.get_buffer(file.get_len())
	self.Waypoint_data.from_bytes(data)
	if !Waypoint.PB_ERR.NO_ERRORS:
		print("OK")
	else:
		print(Waypoint.PB_ERR)
	parse_Waypoint()

var route_scene = load("res://Route.tscn")
var icon_scene = load("res://Icon.tscn")
var path2d_scene = load("res://Route2D.tscn")
var gizmo_scene = load("res://Gizmo/PointEdit.tscn")

##########Gizmo Stuff###########
# How long the ray to search for 3D clickable object should be.
# Shorter is faster but cannot click thing far away.
# Longer is slower but can click things farther away.
var ray_length = 1000

# The last object that was hovered by the mouse. May be null, may be a dangling
# reference to a non-existant node.
var last_hover = null

# The last object that was selected by a click. May be null, may be a dangling
# reference to a non-existant node.
var last_selected = null

################################################################################
# Hand the mouse input of clicking and hovering over an object
################################################################################
func _unhandled_input(event):
	# If the left mouse button is clicked.
	if event is InputEventMouseButton and event.pressed and event.button_index == 1:
		# Emit a ray from the mouse position to see if it intersects with any
		# clickable items.
		var camera = $CameraMount/Camera
		var start_coordinate = camera.project_ray_origin(event.position)
		var end_coordinate = start_coordinate + camera.project_ray_normal(event.position) * self.ray_length
		var space_state = get_world().direct_space_state
		var result = space_state.intersect_ray(start_coordinate, end_coordinate)

		# If if the ray intersects with a node then the mouse successfully
		# clicked something.
		if result:
			if is_instance_valid(self.last_selected) and self.last_selected.has_method("clear_selection") and self.last_selected != result["collider"].get_parent():
				last_selected.clear_selection()
			# Call the select() function on the object that was clicked
			result["collider"].select(camera, event)
			self.last_selected = result["collider"].get_parent()
		else:
			if is_instance_valid(self.last_selected) and self.last_selected.has_method("clear_selection"):
				last_selected.clear_selection()
			self.last_selected = null

	# If the mouse is moved.
	if event is InputEventMouseMotion:
		# Emit a ray from the mouse position to see if it intersects with any
		# clickable items.
		var camera = $CameraMount/Camera
		var start_coordinate = camera.project_ray_origin(event.position)
		var end_coordinate = start_coordinate + camera.project_ray_normal(event.position) * self.ray_length
		var space_state = get_world().direct_space_state
		var result = space_state.intersect_ray(start_coordinate, end_coordinate)

		# If the ray intersects with a node then the mouse is visually hovering
		# over something.
		if result:
			# If there is something that is currently hovered then unhover it
			if is_instance_valid(self.last_hover) and self.last_hover.has_method("unhover") and self.last_hover != result["collider"]:
				self.last_hover.unhover()
			# Call the hover function on the new object that is being hovered
			result["collider"].hover()
			self.last_hover = result["collider"]
		# If nothing is hovere then try to unhover the object and set the
		# currently hovered object to null.
		else:
			if is_instance_valid(self.last_hover) and self.last_hover.has_method("unhover"):
				self.last_hover.unhover()
			self.last_hover = null
			
			
################################################################################
#
################################################################################
func clear_map_markers():
	# Clear all the rendered assets to make way for the new ones
	for path in paths.get_children():
		path.queue_free()

	for path2d in minimap.get_children():
		path2d.queue_free()

	for icon in icons.get_children():
		icon.queue_free()


func init_category_tree():
	self.root = self.marker_packs.create_item()
	root.set_text(0, "Markers available on current map")
	root.set_selectable(0, false)
	root.set_text(1, "Visible")


func parse_Waypoint():
	for category in self.Waypoint_data.get_category():
		parse_category(root, category, category.get_name(), false)


func parse_category(item: TreeItem, category, full_category_name: String, collapsed: bool):
	var category_item = self.marker_packs.create_item(item)
	if category.get_name() == "": 
		# If this is called, there is an error in the Waypoint data
		category_item.set_text(0, "No name")
		category_item.set_metadata(0, "")
		print("Category found with no name.")
		return
	category_item.set_text(0, category.get_display_name())
	category_item.set_metadata(0, full_category_name)
	category_item.set_cell_mode(1, TreeItem.CELL_MODE_CHECK)
	category_item.set_checked(1, Settings.local_category_data.get(full_category_name, {}).get("checked", false))
	category_item.set_tooltip(1, "Show/Hide")
	category_item.set_editable(1, true)
	category_item.set_collapsed(collapsed)

	for path in category.get_trail():
		var path_points := PoolVector3Array()
		var trail_data = path.get_trail_data()
		if trail_data.get_points_x().size() != trail_data.get_points_y().size() or trail_data.get_points_x().size() != trail_data.get_points_z().size():
			print("Warning: Trail ", full_category_name, " does not have equal number of X, Y, and Z coordinates.")
		for index in range(0, trail_data.get_points_z().size()):
			path_points.append(Vector3(trail_data.get_points_x()[index], trail_data.get_points_y()[index], trail_data.get_points_z()[index]))
		var texture_path = path.get_texture_path()
		if texture_path == null:
			print("Warning: No texture found in " , full_category_name)
			continue
		var full_texture_path = self.marker_file_dir.get_current_dir() + texture_path.get_path()
		gen_new_path(path_points, full_texture_path, path, category_item)

	for icon in category.get_icon():
		var position = icon.get_position()
		if position == null:
			print("Warning: No position found for icon ", full_category_name)
			continue
		var position_vector = Vector3(position.get_x(), position.get_y(), position.get_z())
		var texture_path = icon.get_texture_path()
		if texture_path == null:
			print("Warning: No texture found in " , full_category_name)
			continue
		var full_texture_path = self.marker_file_dir.get_current_dir() + texture_path.get_path()
		gen_new_icon(position_vector, full_texture_path, icon, category_item)
	
	for category_child in category.get_children():
		parse_category(category_item, category_child, full_category_name + "." + category_child.get_name(), true)


func apply_category_visibility_to_nodes(category_item: TreeItem):
	Settings.local_category_data[category_item.get_metadata(0)] = {
		"checked" : category_item.is_checked(1),
	} 
	Settings.save()
	var temporary_cateogry_visibility_data = populate_update_dict(category_item, {})
	update_node_visibility(temporary_cateogry_visibility_data, self.paths)
	update_node_visibility(temporary_cateogry_visibility_data, self.icons)


# Builds a dictionary of the visibility of a specific category and all children
func populate_update_dict(category_item: TreeItem, category_visibility_data):
	category_visibility_data[category_item.get_metadata(0)] = is_category_visible(category_item)
	var child_item = category_item.get_children()
	while child_item != null:
		category_visibility_data = populate_update_dict(child_item, category_visibility_data)
		child_item = child_item.get_next()
	return category_visibility_data

#Updates the visibilty of a node and all children.
func update_node_visibility(cateogry_data, nodes):
	for node in nodes.get_children():
		var node_name = node.waypoint.get_category().get_name()
		if node_name in cateogry_data:
			if cateogry_data[node_name]:
				node.visible = true
			else:
				node.visible = false
			if node.get_name() == "Path":
				var index = node.get_index()
				var route2d = self.minimap.get_child(index)
				route2d.visible= node.visible 

#Child visibility is contigent on all parents having permission 
func is_category_visible(category_item: TreeItem) -> bool:
	if category_item == marker_packs.get_root():
		return true
	if category_item == null:
		return false
	if category_item.is_checked(1):
		return is_category_visible(category_item.get_parent())
	else:
		return false


func gen_new_path(points: Array, texture_path: String, waypoint_trail, category_item: TreeItem):
	# Create the texture to use from an image file
	# TODO: We want to be able to cache this data so that if a texture is used
	# by multiple objects we only need to keep ony copy of it in memory. #22.
	# TODO: We want to have two copies of each texture in memory one for 2D 
	# which does not use srgb to render properly, and one for 3D which forces
	# srgb to render properly. Issue #23.
	var texture_file = File.new()
	var image = Image.new()
	if !texture_file.file_exists(texture_path):
		print("Warning: File does not exist: ", texture_path)
	texture_file.open(texture_path, File.READ)
	image.load_png_from_buffer(texture_file.get_buffer(texture_file.get_len()))
	texture_file.close()
	image.lock()
	var texture = ImageTexture.new()
	texture.storage = ImageTexture.STORAGE_COMPRESS_LOSSLESS
	texture.create_from_image(image, 22)

	var new_route = route_scene.instance()
	new_route.texture_path = texture_path # Save the location of the image for later
	
	var points_3d := PoolVector3Array()
	for point in points:
		points_3d.append(Vector3(point[0], point[1], -point[2]))
	
	new_route.create_mesh(points_3d)
	new_route.set_texture(texture)
	new_route.waypoint = waypoint_trail
	if category_item != null:
		new_route.visible = is_category_visible(category_item)
	else:
		new_route.visible = false
	
	paths.add_child(new_route)
	
	# Create a new 2D Path
	var new_2d_path = path2d_scene.instance()
	var points_2d := PoolVector2Array()
	for point in points:
		points_2d.append(Vector2(point[0], -point[2]))
	new_2d_path.points = points_2d
	new_2d_path.texture = texture
	new_2d_path.visible = new_route.visible
	minimap.add_child(new_2d_path)


func gen_new_icon(position: Vector3, texture_path: String, waypoint_icon, category_item: TreeItem): 
	position.z = -position.z
	var new_icon = icon_scene.instance()
	new_icon.translation = position
	new_icon.set_icon_image(texture_path)
	new_icon.waypoint = waypoint_icon
	if category_item != null:
		new_icon.visible = is_category_visible(category_item)
	else:
		new_icon.visible = false
	icons.add_child(new_icon)



################################################################################
# Adjustment and gizmo functions
################################################################################
# The adjust nodes button creates handles at all the node points to allow for
# editing of them via in-game interface. (Nodes can only be edited if the input
# passthrough is blocked, thus preventing any camera movement or player movement
################################################################################
var adjusting = false
func _on_AdjustNodesButton_pressed():
	$Control/Dialogs/NodeEditorDialog.show()
	set_maximal_mouse_block()
	gen_adjustment_nodes()


func gen_adjustment_nodes():
	if self.currently_active_category == null:
		print("No category selected")
		return

	for index in range(self.paths.get_child_count()):
		var route = self.paths.get_child(index)
		var path2d = self.minimap.get_child(index)
		if self.currently_active_category.get_metadata(0) == route.waypoint.get_category().get_name():
			for i in range(route.get_point_count()):
				var gizmo_position = route.get_point_position(i)
			
				# Simplistic cull to prevent nodes that are too far away to be
				# visible from being created. Additional work can be done here
				# if this is not enough of an optimization in the future.
				if (gizmo_position.distance_squared_to(self.correct_player_position) > 10000):
					continue
				var new_gizmo = gizmo_scene.instance()
				new_gizmo.translation = gizmo_position
				new_gizmo.link_point("path", route, path2d, i)
				new_gizmo.connect("selected", self, "on_gizmo_selected")
				new_gizmo.connect("deselected", self, "on_gizmo_deselected")
				$Gizmos.add_child(new_gizmo)

	for index in range(self.icons.get_child_count()):
		var icon = self.icons.get_child(index)
		if self.currently_active_category.get_metadata(0) == icon.waypoint.get_category().get_name():
			var new_gizmo = gizmo_scene.instance()
			new_gizmo.translation = icon.translation
			new_gizmo.link_point("icon", icon)
			new_gizmo.connect("selected", self, "on_gizmo_selected")
			new_gizmo.connect("deselected", self, "on_gizmo_deselected")
			$Gizmos.add_child(new_gizmo)


var currently_selected_node = null
func on_gizmo_selected(object):
	self.currently_selected_node = object
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/DeleteNode.disabled = false
	# Only enable these buttons if the object selected is a point on the path not an icon
	if object.point_type == "path":
		$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/NewNodeAfter.disabled = false
		$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/ReversePathDirection.disabled = false
		$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/SetActivePath.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/SnapSelectedToPlayer.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/XZSnapToPlayer.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/YSnapToPlayer.disabled = false


func on_gizmo_deselected(object):
	self.currently_selected_node = null
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/DeleteNode.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/NewNodeAfter.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/SnapSelectedToPlayer.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/XZSnapToPlayer.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/YSnapToPlayer.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/SetActivePath.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/ReversePathDirection.disabled = true


func clear_adjustment_nodes():
	for child in $Gizmos.get_children():
		$Gizmos.remove_child(child)
		child.queue_free()

################################################################################
# Signal Functions
################################################################################
func _on_main_menu_toggle_pressed():
	$Control/Dialogs/MainMenu.show()
	set_maximal_mouse_block()

func _on_FileDialog_file_selected(path):
	pass

func _on_Dialog_hide():
	for dialog in $Control/Dialogs.get_children():
		if dialog.visible:
			return
	set_minimal_mouse_block()


func _on_LoadPath_pressed():
	if $Control/Dialogs/MarkerPacks.is_visible():
		$Control/Dialogs/MarkerPacks.hide()
	else:
		$Control/Dialogs/MarkerPacks.show()


func _on_RangesButton_pressed():
	$Control/Dialogs/RangesDialog.show()


func _on_PathResolution_value_changed(value):
	path_resolution = value
	for path in $Paths.get_children():
		var csg:CSGPolygon = path.get_node("CSGPolygon")
		csg.path_interval = path_resolution
		csg.material.set_shader_param("interval", path_resolution)


func _on_CloseEditorQuickPanel_pressed():
	$Control/GlobalMenuButton/EditorQuckPanel.hide()
	edit_panel_open = false


func _on_OpenEditorQuickPanel_pressed():
	$Control/GlobalMenuButton/EditorQuckPanel.show()
	edit_panel_open = true


func _on_ChangeTexture_pressed():
	$Control/Dialogs/TexturePathOpen.show()
	var texture_path_dialog:FileDialog = $Control/Dialogs/TexturePathOpen
	texture_path_dialog.set_current_dir(texture_path_dialog.current_dir)
	set_maximal_mouse_block()

################################################################################
# Set the file that will be used to create a new path or icon when a new path
# or icon is created.
################################################################################
func _on_TexturePathOpen_file_selected(path):
	self.next_texture_path = path

################################################################################
# Null out the currently active path so that a new one is created the next time
# a path node is created.
################################################################################
func _on_NewPath_pressed():
	self.currently_active_path = null

################################################################################
# Create a new icon and give it the texture
################################################################################
func _on_NewIcon_pressed():
	var waypoint_icon = Waypoint.Icon.new()
	waypoint_icon.new_category().set_name(self.currently_active_category.get_metadata(0))
	gen_new_icon(self.player_position, self.next_texture_path, waypoint_icon, self.currently_active_category)

# A new path point is created
func _on_NewPathPoint_pressed():
	if self.currently_active_path == null:
		var waypoint_trail = Waypoint.Trail.new()
		waypoint_trail.new_category().set_name(self.currently_active_category.get_metadata(0))
		gen_new_path([self.player_position], self.next_texture_path, waypoint_trail, self.currently_active_category)
	else:
		var z_accurate_player_position = player_position
		z_accurate_player_position.z = -z_accurate_player_position.z
		self.currently_active_path.add_point(z_accurate_player_position)
		self.currently_active_path_2d.add_point(Vector2(self.player_position.x, -self.player_position.z))


################################################################################
# 
################################################################################
func _on_SavePath_pressed():
	#TODO: Save to Waypoint
	pass

################################################################################
# TODO: This function will be used when exporting packs
################################################################################
func _on_SaveDialog_file_selected(path):
	pass


func _on_NodeEditorDialog_hide():
	self.currently_selected_node = null
	clear_adjustment_nodes()
	_on_Dialog_hide()


func _on_DeleteNode_pressed():
	if self.currently_selected_node.point_type == "icon":
		self.currently_selected_node.object_link.get_parent().remove_child(self.currently_selected_node.object_link)
		self.currently_selected_node.object_link.queue_free()
	elif self.currently_selected_node.point_type == "path":
		var path =   self.currently_selected_node.object_link
		var path2d = self.currently_selected_node.object_2d_link
		var index =  self.currently_selected_node.object_index

		path.remove_point(index)
		path2d.remove_point(index)
	clear_adjustment_nodes()
	gen_adjustment_nodes()
	on_gizmo_deselected(self.currently_selected_node)


func _on_NewNodeAfter_pressed():
	if self.currently_selected_node.point_type == "icon":
		print("Warning: Cannot add node to icon")
	elif self.currently_selected_node.point_type == "path":
		print("insert path node")
		var path = self.currently_selected_node.object_link
		var path2d = self.currently_selected_node.object_2d_link
		var index = self.currently_selected_node.object_index

		var start = path.get_point_position(index)
		var midpoint = self.player_position
		midpoint.z = -midpoint.z
		if path.get_point_count() > index+1:
			var end = path.get_point_position(index+1)
			midpoint = ((start-end)/2) + end
		
		path.add_point(midpoint, index+1)
		path2d.add_point(Vector2(midpoint.x, midpoint.z), index+1)

		clear_adjustment_nodes()
		gen_adjustment_nodes()
		on_gizmo_deselected(self.currently_selected_node)
		
func _on_XZSnapToPlayer_pressed():
	self.currently_selected_node.translation.x = self.player_position.x
	self.currently_selected_node.translation.z = -self.player_position.z


func _on_YSnapToPlayer_pressed():
	self.currently_selected_node.translation.y = self.player_position.y


func _on_SnapSelectedToPlayer_pressed():
	self.currently_selected_node.translation.x = self.player_position.x
	self.currently_selected_node.translation.z = -self.player_position.z
	self.currently_selected_node.translation.y = self.player_position.y

func _on_SetActivePath_pressed():
	if self.currently_selected_node.point_type == "icon":
		print("Warning: Cannot set icon as active path")
	elif self.currently_selected_node.point_type == "path":
		self.currently_active_path = self.currently_selected_node.object_link
		self.currently_active_path_2d = self.currently_selected_node.object_2d_link


func _on_ReversePathDirection_pressed():
	self.currently_selected_node.object_link.reverse()


func _on_ExitButton_pressed():
	get_tree().quit()


func _on_Settings_pressed():
	var settings_dialog: WindowDialog = $Control/Dialogs/SettingsDialog
	settings_dialog.load_settings()
	settings_dialog.show()


func _on_MarkerPacks_cell_selected():
	var category_item = self.marker_packs.get_selected()
	self.currently_active_category = category_item


func _on_MarkerPacks_item_edited():
	var category_item = self.marker_packs.get_edited()
	apply_category_visibility_to_nodes(category_item)
