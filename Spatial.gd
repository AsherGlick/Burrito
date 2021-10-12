extends Spatial

var server := UDPServer.new()
var peers = []

var map_id:int = 0


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
		print("New Map")
		
		print("Saving Old Map")
		self.markerdata[str(old_map_id)] = data_from_renderview()
		print("Loading New Map")
		gen_map_markers()

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

var markerdata = {}
var marker_file_path = ""
func load_taco_markers(marker_json_file):
	self.marker_file_path = marker_json_file
	
	if is_xml_file(marker_json_file):
		print("Loading XML file from path ", marker_json_file)
		var parsed_taco_tuple = taco_parser.parse_taco_xml(marker_json_file)
		var json_payload = parsed_taco_tuple[0]
		var error_message = parsed_taco_tuple[1]
		if error_message != "":
			print("XML parsing failed with error message: ", error_message)
		self.markerdata = JSON.parse(json_payload).result
	else:
		print("Loading Json file from path ", marker_json_file)
		var file = File.new()
		file.open(marker_json_file, file.READ)
		var text = file.get_as_text()
		self.markerdata = JSON.parse(text).result
	
	relative_textures_to_absolute_textures(marker_file_path.get_base_dir())

	gen_map_markers()

func is_xml_file(input_file):
	return input_file.split(".")[-1] == "xml"

func relative_textures_to_absolute_textures(marker_file_dir):
	for map in markerdata:
		for icon in markerdata[map]["icons"]:
			if !icon["texture"].is_abs_path():
				icon["texture"] = marker_file_dir + "/" + icon["texture"]
			#print("ABS", icon["texture"])
		for path in markerdata[map]["paths"]:
			if !path["texture"].is_abs_path():
				path["texture"] = marker_file_dir + "/" + path["texture"]


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
onready var icons = $Icons
onready var paths = $Paths
onready var minimap = $Control/MiniMap

func gen_map_markers():
	# Clear all the rendered assets to mak way for the new ones
	for path in paths.get_children():
		path.queue_free()

	for path2d in minimap.get_children():
		path2d.queue_free()

	for icon in icons.get_children():
		icon.queue_free()

	# Load the data from the markers
	if str(map_id) in markerdata:
		var map_markerdata = markerdata[str(map_id)]
		for path in map_markerdata["paths"]:
			gen_new_path(path["points"], path["texture"])

		for icon in map_markerdata["icons"]:
			var position = Vector3(icon["position"][0], icon["position"][1], icon["position"][2])
			gen_new_icon(position, icon["texture"])

func gen_new_path(points: Array, texture_path: String):
	var points_2d: PoolVector2Array = [] 


	# Create the texture to use from an image file
	# TODO: We want to be able to cache this data so that if a texture is used
	# by multiple objects we only need to keep ony copy of it in memory. #22.
	# TODO: We want to have two copies of each texture in memory one for 2D 
	# which does not use srgb to render properly, and one for 3D which forces
	# srgb to render properly. Issue #23.
	var texture_file = File.new()
	var image = Image.new()
	texture_file.open(texture_path, File.READ)
	image.load_png_from_buffer(texture_file.get_buffer(texture_file.get_len()))
	texture_file.close()
	image.lock()
	var texture = ImageTexture.new()
	texture.storage = ImageTexture.STORAGE_COMPRESS_LOSSLESS
	texture.create_from_image(image, 22)



	# Create a new 3D route
	var new_route = route_scene.instance()
#	var new_curve = Curve3D.new()
#	for point in points:
#		new_curve.add_point(Vector3(point[0], point[1], -point[2]))
#		points_2d.append(Vector2(point[0], -point[2]))

#	new_path.curve = new_curve
	new_route.texture_path = texture_path # Save the location of the image for later
	#path_3d_markers.append(new_path)
	
	var points_3d := PoolVector3Array()
	for point in points:
		points_3d.append(Vector3(point[0], point[1], -point[2]))
	
	new_route.create_mesh(points_3d)
	new_route.set_texture(texture)
	paths.add_child(new_route)
	
	
	
	
	
	
	
	for point in points:
		points_2d.append(Vector2(point[0], -point[2]))
	
	
	# Create a new 2D Path
	var new_2d_path = path2d_scene.instance()
	new_2d_path.points = points_2d
	new_2d_path.texture = texture
	minimap.add_child(new_2d_path)
	
	self.currently_active_path = new_route
	self.currently_active_path_2d = new_2d_path


################################################################################
#
################################################################################
func gen_new_icon(position: Vector3, texture_path: String):
	position.z = -position.z
	var new_icon = icon_scene.instance()
	new_icon.translation = position
	new_icon.set_icon_image(texture_path)

	#icon_markers.append(new_icon)
	icons.add_child(new_icon)

# This function take all of the currently rendered objects and converts it into
# the data format that is saved/loaded from.
func data_from_renderview():
	var icons_data = []
	var paths_data = []
	
	for icon in $Icons.get_children():
		icons_data.append({
			"position": [icon.translation.x, icon.translation.y, -icon.translation.z],
			"texture": icon.texture_path
		})
	
	for path in $Paths.get_children():
		#print(path)
		var points = []
		for point in range(path.get_point_count()):
			var point_position:Vector3 = path.get_point_position(point)
			points.append([point_position.x, point_position.y, -point_position.z])
		paths_data.append({
			"points": points,
			"texture": path.texture_path
		})

	var data_out = {"icons": icons_data, "paths": paths_data}
	return data_out

func _on_main_menu_toggle_pressed():
	$Control/Dialogs/MainMenu.show()
	set_maximal_mouse_block()

func _on_FileDialog_file_selected(path):
	load_taco_markers(path)


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
	for index in range(self.paths.get_child_count()):
		var route = self.paths.get_child(index)
		var path2d = self.minimap.get_child(index)
		#var curve: Curve3D = path.curve
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


func _on_Dialog_hide():
	for dialog in $Control/Dialogs.get_children():
		if dialog.visible:
			return
	set_minimal_mouse_block()


func _on_LoadPath_pressed():
	var open_dialog: FileDialog = $Control/Dialogs/FileDialog
	open_dialog.show()
	open_dialog.set_current_dir(open_dialog.current_dir)


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
	gen_new_icon(self.player_position, self.next_texture_path)

# A new path point is created
func _on_NewPathPoint_pressed():
	if self.currently_active_path == null:
		gen_new_path([self.player_position], self.next_texture_path)
	else:
		var z_accurate_player_position = player_position
		z_accurate_player_position.z = -z_accurate_player_position.z
		self.currently_active_path.add_point(z_accurate_player_position)
		self.currently_active_path_2d.add_point(Vector2(self.player_position.x, -self.player_position.z))



################################################################################
# open the save dialog window. When a path is selected
# _on_SaveDialog_file_selected() will be called with the user specified path.
################################################################################
func _on_SavePath_pressed():
	$Control/Dialogs/SaveDialog.show()

################################################################################
# Save the current markers to a file, this includes all markers in memory not
# just the markers on the current map.
################################################################################
func _on_SaveDialog_file_selected(path):
	self.markerdata[str(self.map_id)] = data_from_renderview()
	var save_game = File.new()
	save_game.open(path, File.WRITE)
	save_game.store_string(JSON.print(self.markerdata))


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

