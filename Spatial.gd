extends Spatial

var server := UDPServer.new()
var peers = []

var map_id:int = 0

var map_is_open: bool
var compass_is_top_right: bool

var edit_panel_open: bool = false

# This is the path to the texture that will be used for the next created 3d-path
# object or icon object in the UI
var next_texture_id: int = 0

# A pointer to the trail object that any new nodes should be appended to. If
# the value is null then a new trail will be created the next time a trail point
# will be created.
var currently_active_trail3d = null
var currently_active_trail2d = null
var currently_active_guildpoint_trail = null
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
var trail_resolution = 1

#x11 fg and window id
var x11_fg: X11_FG
var taco_parser: TacoParser
var x11_window_id_burrito: int
var is_transient:bool = false

# Scenes used throughout this scene
const trail3d_scene = preload("res://Trail3D.tscn")
const icon_scene = preload("res://Icon.tscn")
const category3d_scene = preload("res://Category3D.tscn")
const category2d_scene = preload("res://Category2D.tscn")
const trail2d_scene = preload("res://Trail2D.tscn")
const gizmo_scene = preload("res://Gizmo/PointEdit.tscn")

# Scripts containing code used by this scene
const CategoryData = preload("res://CategoryData.gd")
const Guildpoint = preload("res://guildpoint.gd")
const FileHandler = preload("res://FileHandler.gd")

# File path for the the json that contains a hash of the data files
const HASH_BY_MAP_ID_FILEPATH: String = "user://hash_by_map_id.json"

##########Node Connections###########
onready var markers_ui := $Control/Dialogs/CategoriesDialog/MarkersUI as Tree
onready var markers_3d := $Markers3D as Spatial
onready var markers_2d := $Control/Markers2D as Node2D
onready var overwrite_confirmation := $Control/Dialogs/ImportPackDialogs/OverwriteConfirm as ConfirmationDialog

# Called when the node enters the scene tree for the first time.
func _ready():
	self.markers_ui.set_column_expand(1, false)
	self.markers_ui.set_column_min_width(1, 24)
	get_tree().get_root().set_transparent_background(true)
	x11_fg = X11_FG.new()
	taco_parser = TacoParser.new()
	x11_window_id_burrito = OS.get_native_handle(OS.WINDOW_HANDLE)
	OS.window_maximized = false
	# Start off with a small size before GW2 client is up

	if Settings.override_size_enabled:
		OS.window_size = Vector2(Settings.override_size_width, Settings.override_size_height)
	else:
		OS.window_size = Vector2(Settings.minimum_width, Settings.minimum_height)

	# Postion at top left corner
	OS.set_window_position(Vector2(0,0))
	set_minimal_mouse_block()

	server.listen(4242)

	if (Settings.burrito_link_auto_launch_enabled):
		launch_burrito_link()

	if (Settings.start_with_open_menu):
		_on_main_menu_toggle_pressed()

################################################################################
# show_error
#
# This function prints a user error out. Currently it prints to stdout but may
# one day be shown to the user.
################################################################################
func show_user_error(error_string: String):
	print(error_string)


# The process id of burrito link if it is launched automatically by burrito
var burrito_link_process_id = 0

################################################################################
# launch_burrito_link
#
# This function launches the burrito link binary using the values for it that
# are saved in "settings".
################################################################################
func launch_burrito_link():
		for env_arg in Settings.burrito_link_env_args.split("\n"):
			env_arg = env_arg.trim_prefix("export ")
			var key_values = env_arg.split('=', true, 1)

			if len(key_values) != 2:
				show_user_error("Invalid burrito_link environment arg: " + env_arg)
				return

			var key = key_values[0]
			var value = key_values[1].trim_prefix('"').trim_suffix('"')
			OS.set_environment(key, value)

		# Launch burrito link with a 2 hour timeout
		# If burrito crashes then burrito_link will automatically exit at the timeout.
		# If burrito does not crash and the timeout expires then burrito will relaunch burrito_link automatically
		burrito_link_process_id = OS.execute(Settings.burrito_link_wine_path, ["burrito_link/burrito_link.exe", "--timeout", "7200"], false)

func close_burrito_link():
	if (burrito_link_process_id != 0):
		OS.kill(burrito_link_process_id)
		burrito_link_process_id = 0


func exit_burrito():
	if Settings.burrito_link_auto_launch_enabled:
		close_burrito_link()
	get_tree().quit()


func set_minimal_mouse_block():
	var menu_button = $Control/GlobalMenuButton
	var top_corner = menu_button.get_position()
	var bottom_corner = menu_button.get_position() + menu_button.get_size()

	if self.edit_panel_open:
		var editor_panel = $Control/GlobalMenuButton/EditorQuckPanel
		bottom_corner = menu_button.get_position() + editor_panel.get_position() + editor_panel.get_size()

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
			elif packet_type == 3:
				decode_timeout_packet(spb)
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
		self.markers_3d.visible = !map_is_open
		reset_minimap_masks(false)
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
			map_corner = get_viewport().size - Vector2(compass_width, compass_height + Settings.minimap_scale[Settings.ui_size]["offset"])
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
		$Control/Markers2D.rotation = map_rotation
	else:
		$Control/Markers2D.rotation = 0

	var map_midpoint = map_size/2 + map_corner;

	$Control/Markers2D.scale=Vector2(map_object_scaling, map_object_scaling)
	var map_translation = map_offset
	$Control/Markers2D.position = (map_translation / map_scale) + map_midpoint - player_map_position + delta_position
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

	var size = Vector2(Settings.minimum_width, Settings.minimum_height)
	if Settings.override_size_enabled:
		size.x = Settings.override_size_width
		size.y = Settings.override_size_height
	else:
		var size_tuple = x11_fg.get_window_geometry(x11_window_id_gw2)
		size.x = size_tuple[0]
		size.y = size_tuple[1]

		if size.x < Settings.minimum_width:
			size.x = Settings.minimum_width
		if size.y < Settings.minimum_height:
			size.y = Settings.minimum_height

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

	# Calculations to dynamically place the main icon/button
	Settings.ui_size = int(identity["uisz"])
	# If the value is not part of the dictionary use the "normal" size.
	if !Settings.button_margin.has(Settings.ui_size):
		Settings.ui_size = 1

	$Control/GlobalMenuButton._update_global_menu_button_position()

	if !is_any_dialog_visible():
		set_minimal_mouse_block()

	compass_width = compass_width * Settings.minimap_scale[Settings.ui_size]["factor"]
	compass_height = compass_height * Settings.minimap_scale[Settings.ui_size]["factor"]

	if self.map_id != old_map_id:
		print("New Map")
		var old_texture_path: String = ""
		old_texture_path = get_texture_path(self.next_texture_id)
		if old_map_id != 0 and not read_hash(old_map_id) == make_hash(self.guildpoint_data.to_bytes()):
			print("Saving Old Map")
			save_map_data(old_map_id)
		print("Loading New Map")
		load_guildpoint_markers(self.map_id)
		self.next_texture_id = get_texture_index(old_texture_path)
		if self.next_texture_id == -1:
			self.guildpoint_data.add_textures().set_filepath(old_texture_path)
			self.next_texture_id = self.guildpoint_data.get_textures().size() - 1

	reset_minimap_masks()



func decode_timeout_packet(spb: StreamPeerBuffer):
	if Settings.burrito_link_auto_launch_enabled:
		print("Link Timeout Reached, should restart link if started by burrito automatically")
		close_burrito_link()
		launch_burrito_link()


func reset_minimap_masks(reset_3d: bool = true):
	var viewport_size = get_viewport().size
	var compass_corner1 = Vector2(0, 0)
	var compass_corner2 = viewport_size
	if !map_is_open && !compass_is_top_right:
		compass_corner1 = Vector2(viewport_size.x-compass_width, Settings.minimap_scale[Settings.ui_size]["offset"])
		compass_corner2 = compass_corner1 + Vector2(compass_width, compass_height)
	elif !map_is_open && compass_is_top_right:
		compass_corner1 = viewport_size - Vector2(self.compass_width, self.compass_height)
		compass_corner2 = compass_corner1 + Vector2(self.compass_width, self.compass_height)

	for category in self.markers_2d.subcategories:
		reset_2D_minimap_masks(category, compass_corner1, compass_corner2)

	if reset_3d:
		for category in self.markers_3d.subcategories:
			reset_3D_minimap_masks(category)

func reset_2D_minimap_masks(category2d: Node2D, compass_corner1: Vector2, compass_corner2: Vector2):
	for trail2d in category2d.trails2d:
		trail2d.material.set_shader_param("minimap_corner", compass_corner1)
		trail2d.material.set_shader_param("minimap_corner2", compass_corner2)
	for subcategory in category2d.subcategories:
		reset_2D_minimap_masks(subcategory, compass_corner1, compass_corner2)

func reset_3D_minimap_masks(category: Spatial):
	for trail in category.trails3d:
		trail.get_node("MeshInstance").material_override.set_shader_param("map_size", Vector2(self.compass_width, self.compass_height))
	for icon in category.icons:
		icon.material_override.set_shader_param("map_size", Vector2(self.compass_width, self.compass_height))
	for subcategory in category.subcategories:
		reset_3D_minimap_masks(subcategory)


var guildpoint_data = Guildpoint.Guildpoint.new()
var marker_file_path = ""

func load_guildpoint_markers(map_id_to_load: int):
	self.marker_file_path = Settings.get_unsaved_markers_dir() + String(map_id_to_load) + ".guildpoint"
	self.guildpoint_data = Guildpoint.Guildpoint.new()
	clear_map_markers()
	init_category_tree()
	var file = File.new()
	print("Loading protobuf file from path ", self.marker_file_path)
	file.open(self.marker_file_path, file.READ)
	var data = file.get_buffer(file.get_len())
	self.guildpoint_data.from_bytes(data)
	if !Guildpoint.PB_ERR.NO_ERRORS:
		print("OK")
	else:
		print(Guildpoint.PB_ERR)
	guildpoint_categories_to_godot_nodes()


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
	self.markers_3d.clear_all()
	self.markers_2d.clear_all()

func init_category_tree():
	self.markers_ui.clear()
	var root : TreeItem
	root = self.markers_ui.create_item()
	root.set_text(0, "Markers")
	root.set_expand_right(0, true)



func guildpoint_categories_to_godot_nodes():
	for guildpoint_category in self.guildpoint_data.get_category():
		_guildpoint_categories_to_godot_nodes(null, guildpoint_category, self.markers_3d, self.markers_2d, 0)


func _guildpoint_categories_to_godot_nodes(item: TreeItem, guildpoint_category: Guildpoint.Category, parent_category3d: Spatial, parent_category2d: Node2D, depth: int):
	var godot_category3d = category3d_scene.instance()
	var godot_category2d = category2d_scene.instance()
	parent_category3d.add_subcategory(godot_category3d)
	parent_category2d.add_subcategory(godot_category2d)

	var category_item: TreeItem = self.markers_ui.create_item(item)
	var category_data = CategoryData.new()
	category_data.guildpoint_category = guildpoint_category
	category_data.category3d = godot_category3d
	category_data.category2d = godot_category2d

	category_item.set_metadata(0, category_data)
	var category_name: String = guildpoint_category.get_name()
	if category_name == "":
		print("Category found with no name.")
		category_name = "No Name"
	category_item.set_text(0, category_name)
	category_item.set_cell_mode(1, TreeItem.CELL_MODE_CHECK)
	var checked = true
	if depth in [0, 1]:
		checked = false
	category_item.set_checked(1, Settings.local_category_data.get(Marshalls.raw_to_base64(category_data.guildpoint_category.get_id()), {}).get("checked", checked))
	category_item.set_tooltip(1, "Show/Hide")
	category_item.set_editable(1, true)
	var collapsed = true
	if depth == 0:
		collapsed = false
	category_item.set_collapsed(collapsed)
	category_item.set_selectable(1, false)

	category_data.is_visible = category_item.is_checked(1)
	godot_category3d.visible = category_data.is_visible
	godot_category2d.visible = category_data.is_visible

	for trail in guildpoint_category.get_trail():
		gen_new_trail(trail, category_item)

	for icon in guildpoint_category.get_icon():
		gen_new_icon(icon, category_item)


	for category_child in guildpoint_category.get_children():
		_guildpoint_categories_to_godot_nodes(category_item, category_child, godot_category3d, godot_category2d, depth + 1)


func apply_category_visibility_to_nodes(category_item: TreeItem):
	var category_data = category_item.get_metadata(0)
	var relative_path: String = self.markers_3d.get_path_to(category_data.category3d)
	Settings.local_category_data[Marshalls.raw_to_base64(category_data.guildpoint_category.get_id())] = {
		"checked" : category_item.is_checked(1),
	}
	Settings.save()

	category_data.is_visible = category_item.is_checked(1)
	category_data.category3d.visible = category_data.is_visible
	category_data.category2d.visible = category_data.is_visible


func gen_new_trail(guildpoint_trail: Guildpoint.Trail, category_item: TreeItem) -> Array:
	# Create the texture to use from an image file
	# TODO: We want to be able to cache this data so that if a texture is used
	# by multiple objects we only need to keep ony copy of it in memory. #22.
	# TODO: We want to have two copies of each texture in memory one for 2D
	# which does not use srgb to render properly, and one for 3D which forces
	# srgb to render properly. Issue #23.
	var texture_id: int = guildpoint_trail.get_texture_id()
	if texture_id == 0:
		var category_data = category_item.get_metadata(0)
		print("Warning: No texture found in " , category_data.guildpoint_category.get_name())
	# TODO(330): Error Textures
	var texture_path: String = Settings.get_unsaved_markers_dir() + get_texture_path(texture_id)
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

	var new_trail3d = trail3d_scene.instance()
	new_trail3d.guildpoint = guildpoint_trail
	new_trail3d.refresh_mesh()
	new_trail3d.set_texture(texture)
	var category_data = category_item.get_metadata(0)
	category_data.category3d.add_trail3d(new_trail3d)


	# Create a new 2D Trail
	var new_trail2d = trail2d_scene.instance()
	var points_2d := PoolVector2Array()
	new_trail2d.texture = texture
	new_trail2d.guildpoint = guildpoint_trail
	new_trail2d.refresh_points()
	category_data.category2d.add_trail2d(new_trail2d)

	return [new_trail3d, new_trail2d]

func gen_new_icon(guildpoint_icon: Guildpoint.Icon, category_item: TreeItem):
	var texture_id: int = guildpoint_icon.get_texture_id()
	if texture_id == 0:
		var category_data = category_item.get_metadata(0)
		print("Warning: No texture found in " , category_data.guildpoint_category.get_name())
	# TODO(330) Error Textures
	var texture_path: String = Settings.get_unsaved_markers_dir() + get_texture_path(texture_id)
	var position = guildpoint_icon.get_position()
	if position == null:
		var category_data = category_item.get_metadata(0)
		print("Warning: No position found for icon ", category_data.guildpoint_category.get_name())
		return
	var position_vector = Vector3(position.get_x(), position.get_y(), -position.get_z())
	var new_icon = icon_scene.instance()
	new_icon.translation = position_vector
	new_icon.set_icon_image(texture_path)
	new_icon.guildpoint = guildpoint_icon
	var category_data = category_item.get_metadata(0)
	category_data.category3d.add_icon(new_icon)

################################################################################
# Section of functions for saving data to file
################################################################################
func save_map_data(map_id: int):
	var packed_bytes = self.guildpoint_data.to_bytes()
	var file = File.new()
	file.open(self.marker_file_path, file.WRITE)
	file.store_buffer(packed_bytes)

func make_hash(data: PoolByteArray) -> String:
	var ctx = HashingContext.new()
	ctx.start(HashingContext.HASH_MD5)
	ctx.update(data)
	var res: PoolByteArray = ctx.finish()
	return res.hex_encode()


# Save all hashes
func save_hashes():
	var file = File.new()
	var data = {}
	var dir = Directory.new()
	dir.open(Settings.get_unsaved_markers_dir())
	dir.list_dir_begin()
	var file_name = dir.get_next()
	while file_name != "":
		if dir.file_exists(file_name):
			file.open(file_name, File.READ)
			data[file_name.get_basename()] = make_hash(file.get_buffer(file.get_len()))
		file_name = dir.get_next()
	file.open(HASH_BY_MAP_ID_FILEPATH, File.WRITE)
	file.store_string(JSON.print(data))

func read_hash(map_id: int) -> String:
	var file = File.new()
	if not file.file_exists(HASH_BY_MAP_ID_FILEPATH):
		return ""
	file.open(HASH_BY_MAP_ID_FILEPATH, File.READ)
	return JSON.parse(file.get_as_text()).result.get(String(map_id), "")

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
	var guildpoint_category = self.currently_active_category.get_metadata(0).guildpoint_category
	var category3d = self.currently_active_category.get_metadata(0).category3d
	var category2d = self.currently_active_category.get_metadata(0).category2d
	for trail_index in guildpoint_category.get_trail().size():
		var guildpoint_trail = guildpoint_category.get_trail()[trail_index]
		var trail3d = category3d.trails3d[trail_index]
		var trail2d = category2d.trails2d[trail_index]
		for point_index in get_trail_point_count(guildpoint_trail):
			var gizmo_position = get_trail_point_position(guildpoint_trail, point_index)
			# Simplistic cull to prevent nodes that are too far away to be
			# visible from being created. Additional work can be done here
			# if this is not enough of an optimization in the future.
			if (gizmo_position.distance_squared_to(self.correct_player_position) > 10000):
				continue
			var new_gizmo = gizmo_scene.instance()
			new_gizmo.translation = gizmo_position
			new_gizmo.connect("selected", self, "on_trail_gizmo_selected", [guildpoint_trail, trail3d, trail2d, point_index])
			new_gizmo.connect("deselected", self, "on_gizmo_deselected")
			new_gizmo.connect("updated", self, "set_trail_point_position", [guildpoint_trail, trail3d, trail2d, point_index])
			$Gizmos.add_child(new_gizmo)
	for icon_index in guildpoint_category.get_icon().size():
		var guildpoint_icon = guildpoint_category.get_icon()[icon_index]
		var icon = category3d.icons[icon_index]
		var new_gizmo = gizmo_scene.instance()
		new_gizmo.translation = get_icon_position(guildpoint_icon)
		new_gizmo.connect("selected", self, "on_icon_gizmo_selected", [guildpoint_icon, icon])
		new_gizmo.connect("deselected", self, "on_gizmo_deselected")
		new_gizmo.connect("updated", self, "set_icon_position", [guildpoint_icon, icon])
		$Gizmos.add_child(new_gizmo)

var currently_selected_gizmo = null
var currently_selected_icon = null
var currently_selected_guildpoint_icon = null
var currently_selected_guildpoint_trail = null
var currently_selected_trail3d = null
var currently_selected_trail2d = null
var currently_selected_point_index = null

func set_2D_position_from_3D_point(position: Vector3, trail2d: Line2D, index: int):
	trail2d.set_point_position(index, Vector2(position.x, position.z))

func on_icon_gizmo_selected(object: Spatial, guildpoint_icon: Guildpoint.Icon, icon: Sprite3D):
	self.currently_selected_gizmo = object
	self.currently_selected_icon = icon
	self.currently_selected_guildpoint_icon = guildpoint_icon
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/DeleteNode.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/SnapSelectedToPlayer.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/XZSnapToPlayer.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/YSnapToPlayer.disabled = false


func on_trail_gizmo_selected(object: Spatial, guildpoint_trail: Guildpoint.Trail, trail3d: Spatial, trail2d: Line2D, point_index: int):
	self.currently_selected_gizmo = object
	self.currently_selected_guildpoint_trail = guildpoint_trail
	self.currently_selected_trail3d = trail3d
	self.currently_selected_trail2d = trail2d
	self.currently_selected_point_index = point_index
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/DeleteNode.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/NewTrailPointAfter.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/ReverseTrailDirection.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/SetActiveTrail.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/SnapSelectedToPlayer.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/XZSnapToPlayer.disabled = false
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/YSnapToPlayer.disabled = false


func on_gizmo_deselected():
	self.currently_selected_gizmo = null
	self.currently_selected_icon = null
	self.currently_selected_guildpoint_icon = null
	self.currently_selected_guildpoint_trail = null
	self.currently_selected_trail3d = null
	self.currently_selected_trail2d = null
	self.currently_selected_point_index = null
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/DeleteNode.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/NewTrailPointAfter.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/SnapSelectedToPlayer.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/XZSnapToPlayer.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/YSnapToPlayer.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/SetActiveTrail.disabled = true
	$Control/Dialogs/NodeEditorDialog/ScrollContainer/VBoxContainer/ReverseTrailDirection.disabled = true


func clear_adjustment_nodes():
	for child in $Gizmos.get_children():
		$Gizmos.remove_child(child)
		child.queue_free()

################################################################################
# Update Guildpoint datum
################################################################################
func get_texture_index(path: String) -> int:
	if path == "":
		return 0
	for i in self.guildpoint_data.get_textures().size():
		if path == self.guildpoint_data.get_textures()[i].get_filepath():
			return i
	return -1

func get_texture_path(texture_id: int) -> String:
	if texture_id == 0:
		return ""
	if texture_id >= self.guildpoint_data.get_textures().size() or texture_id < 0:
		toast("Invalid texture index found")
		# TODO(330): This should return an error texture filepath instead of empty string
		return ""
	return self.guildpoint_data.get_textures()[texture_id].get_filepath()

func set_icon_position(new_position: Vector3, guildpoint_icon: Guildpoint.Icon, icon: Sprite3D):
	if icon.guildpoint != guildpoint_icon:
		push_error("Desync between Guildpoint and Icon")
	var position = guildpoint_icon.new_position()
	position.set_x(new_position.x)
	position.set_y(new_position.y)
	position.set_z(new_position.z)
	icon.set_position(new_position)

func remove_icon(guildpoint_icon: Guildpoint.Icon, icon: Sprite3D):
	if icon.guildpoint != guildpoint_icon:
		push_error("Desync between Guildpoint and Icon")
	var category: Node = icon.get_parent()
	var icon_index: int = category.icons.find(icon)
	category.guildpoint_category.get_icon().remove(icon_index)
	category.category3d.remove_icon(icon)

func get_icon_position(guildpoint_icon: Guildpoint.Icon):
	var position: Vector3
	position[0] = guildpoint_icon.get_position().get_x()
	position[1] = guildpoint_icon.get_position().get_y()
	position[2] = -guildpoint_icon.get_position().get_z()
	return position

func set_trail_point_position(position: Vector3, guildpoint_trail: Guildpoint.Trail, trail3d: Spatial, trail2d: Line2D, point_index: int):
	if trail3d.guildpoint != trail2d.guildpoint or trail2d.guildpoint != guildpoint_trail:
		push_error("Desync between Guildpoint, Trail3D, and Trail2D")
	var trail_data = guildpoint_trail.get_trail_data()
	trail_data.get_points_x()[point_index] = position.x
	trail_data.get_points_y()[point_index] = position.y
	trail_data.get_points_z()[point_index] = -position.z
	refresh_trail3d_points(trail3d)
	refresh_trail2d_points(trail2d)

func reverse_trail(guildpoint_trail: Guildpoint.Trail, trail3d: Spatial, trail2d: Line2D):
	if trail3d.guildpoint != trail2d.guildpoint or trail2d.guildpoint != guildpoint_trail:
		push_error("Desync between Guildpoint, Trail3D, and Trail2D")
	var trail_data = guildpoint_trail.get_trail_data()
	trail_data.get_points_x().invert()
	trail_data.get_points_y().invert()
	trail_data.get_points_z().invert()
	refresh_trail3d_points(trail3d)
	refresh_trail2d_points(trail2d)

func get_trail_point_count(guildpoint_trail: Guildpoint.Trail):
	var trail_data = guildpoint_trail.get_trail_data()
	return trail_data.get_points_x().size()

func get_trail_point_position(guildpoint_trail: Guildpoint.Trail, point_index: int):
	var position: Vector3
	var trail_data = guildpoint_trail.get_trail_data()
	position[0] = trail_data.get_points_x()[point_index]
	position[1] = trail_data.get_points_y()[point_index]
	position[2] = -trail_data.get_points_z()[point_index]
	return position

func add_trail_point(position: Vector3, guildpoint_trail: Guildpoint.Trail, trail3d: Spatial, trail2d: Line2D, point_index: int = -1):
	if trail3d.guildpoint != trail2d.guildpoint or trail2d.guildpoint != guildpoint_trail:
		push_error("Desync between Guildpoint, Trail3D, and Trail2D")
	var trail_data = trail3d.guildpoint.get_trail_data()
	if point_index == -1:
		trail_data.get_points_x().append(position.x)
		trail_data.get_points_y().append(position.y)
		trail_data.get_points_z().append(-position.z)
	else:
		trail_data.get_points_x().insert(point_index, position.x)
		trail_data.get_points_y().insert(point_index, position.y)
		trail_data.get_points_z().insert(point_index, -position.z)
	refresh_trail3d_points(trail3d)
	refresh_trail2d_points(trail2d)

func remove_trail_point(guildpoint_trail: Guildpoint.Trail, trail3d: Spatial, trail2d: Line2D, point_index: int):
	if trail3d.guildpoint != trail2d.guildpoint or trail2d.guildpoint != guildpoint_trail:
		push_error("Desync between Guildpoint, Trail3D, and Trail2D")
	var trail_data = trail3d.guildpoint.get_trail_data()
	trail_data.get_points_x().remove(point_index)
	trail_data.get_points_y().remove(point_index)
	trail_data.get_points_z().remove(point_index)
	refresh_trail3d_points(trail3d)
	refresh_trail2d_points(trail2d)

func new_trail_point_after(guildpoint_trail: Guildpoint.Trail, trail3d: Spatial, trail2d: Line2D, point_index: int):
	var start: Vector3 = get_trail_point_position(guildpoint_trail, point_index)
	var target_position: Vector3
	if get_trail_point_count(guildpoint_trail) > point_index+1:
		var end: Vector3 = get_trail_point_position(guildpoint_trail, point_index+1)
		target_position = ((start-end)/2) + end
	else:
		target_position = Vector3(self.player_position.x, self.player_position.y, -self.player_position.z)
	add_trail_point(target_position, guildpoint_trail, trail3d, trail2d, point_index+1)

func refresh_trail3d_points(trail3d: Spatial):
	trail3d.refresh_mesh()

func refresh_trail2d_points(trail2d: Line2D):
	trail2d.refresh_points()

func toast(message: String):
	print(message)

################################################################################
# Signal Functions
################################################################################
func _on_main_menu_toggle_pressed():
	$Control/Dialogs/MainMenu.show()
	set_maximal_mouse_block()

func is_any_dialog_visible(node = $Control/Dialogs):
	for dialog in node.get_children():
		if dialog.get_class() == "Control":
			if is_any_dialog_visible(dialog):
				return true
		elif dialog.visible:
			return true
	return false

func _on_Dialog_hide():
	if !is_any_dialog_visible():
		set_minimal_mouse_block()


func _on_LoadTrail_pressed():
	if $Control/Dialogs/CategoriesDialog.is_visible():
		$Control/Dialogs/CategoriesDialog.hide()
	else:
		$Control/Dialogs/CategoriesDialog.show()


func _on_RangesButton_pressed():
	$Control/Dialogs/RangesDialog.show()


func _on_TrailResolution_value_changed(value):
	trail_resolution = value
	for trail3d in $Trails.get_children():
		var csg:CSGPolygon = trail3d.get_node("CSGPolygon")
		csg.trail_interval = trail_resolution
		csg.material.set_shader_param("interval", trail_resolution)


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
# Set the file that will be used to create a new trail or icon when a new trail
# or icon is created.
################################################################################
func _on_TexturePathOpen_file_selected(path: String):
	var next_texture_path = FileHandler.find_image_duplicates(path, Settings.get_unsaved_markers_dir())
	if next_texture_path == null:
		FileHandler.create_directory_if_missing(Settings.get_unsaved_markers_dir().plus_file("Data"))
		next_texture_path = "Data".plus_file(path.get_file())
		var file = File.new()
		if file.file_exists(Settings.get_unsaved_markers_dir().plus_file(next_texture_path)):
			toast(String(["Error: A different image with the name ", path.get_file(), " has already been imported. Please rename the file and try again."]))
			# TODO: We "could" exit  here. But we have no mechanism for
			# selecting textures that have already been imported. Instead we
			# will emit this warning and then proceed as if the file was
			# imported correctly. This will function as a method to select the
			# texture that was already imported. It is incredibly hacky and I
			# hate it but I dont want to write a texture selector right now
			# because I want to remake the entire UI anyways and
			# I dont want to build this texture selector twice.
		else:
			FileHandler.copy_file(path, Settings.get_unsaved_markers_dir().plus_file(next_texture_path))
	var texture_index = get_texture_index(next_texture_path)
	if texture_index == -1:
		self.guildpoint_data.add_textures().set_filepath(next_texture_path)
		texture_index = self.guildpoint_data.get_textures().size() - 1
	self.next_texture_id = texture_index


################################################################################
# Null out the currently active trail so that a new one is created the next time
# a trail node is created.
################################################################################
func _on_NewTrail_pressed():
	self.currently_active_trail3d = null
	self.currently_active_trail2d = null
	self.currently_active_guildpoint_trail = null

################################################################################
# Create a new icon and give it the texture
################################################################################
func _on_NewIcon_pressed():
	if self.currently_active_category == null:
		print("No category selected")
		return
	var guildpoint_category: Guildpoint.Category = self.currently_active_category.get_metadata(0).guildpoint_category
	var guildpoint_icon = guildpoint_category.add_icon()
	var position = guildpoint_icon.new_position()
	position.set_x(self.player_position.x)
	position.set_y(self.player_position.y)
	position.set_z(-self.player_position.z)
	guildpoint_icon.set_texture_id(self.next_texture_id)
	gen_new_icon(guildpoint_icon, self.currently_active_category)

# A new trail point is created
func _on_NewTrailPoint_pressed():
	if self.currently_active_trail3d == null:
		if self.currently_active_category == null:
			print("No category selected")
			return
		var guildpoint_category: Guildpoint.Category = self.currently_active_category.get_metadata(0).guildpoint_category
		var guildpoint_trail = guildpoint_category.add_trail()
		var trail_data = guildpoint_trail.new_trail_data()
		trail_data.add_points_x(self.player_position.x)
		trail_data.add_points_y(self.player_position.y)
		trail_data.add_points_z(self.player_position.z)
		guildpoint_trail.set_texture_id(self.next_texture_id)
		self.currently_active_guildpoint_trail = guildpoint_trail
		var new_trails: Array = gen_new_trail(guildpoint_trail, self.currently_active_category)
		self.currently_active_trail3d = new_trails[0]
		self.currently_active_trail2d = new_trails[1]
	else:
		var target_position = Vector3(self.player_position.x, self.player_position.y, -self.player_position.z)
		add_trail_point(target_position, self.currently_active_guildpoint_trail, self.currently_active_trail3d, self.currently_active_trail2d)

func _on_NodeEditorDialog_hide():
	on_gizmo_deselected()
	clear_adjustment_nodes()
	_on_Dialog_hide()


func _on_DeleteNode_pressed():
	if self.currently_selected_guildpoint_icon != null:
		remove_icon(self.currently_selected_guildpoint_icon, self.currently_selected_icon)
	if self.currently_selected_guildpoint_trail != null :
		remove_trail_point(self.currently_selected_guildpoint_trail, self.currently_selected_trail3d, self.currently_selected_trail2d, self.currently_selected_point_index)
	on_gizmo_deselected()
	clear_adjustment_nodes()
	gen_adjustment_nodes()


func _on_NewTrailPointAfter_pressed():
	if self.currently_selected_guildpoint_trail != null:
		new_trail_point_after(self.currently_selected_guildpoint_trail, self.currently_selected_trail3d, self.currently_selected_trail2d, self.currently_selected_point_index)
	on_gizmo_deselected()
	clear_adjustment_nodes()
	gen_adjustment_nodes()

func _on_XZSnapToPlayer_pressed():
	if self.currently_selected_gizmo == null:
		print("Warning: No Point Selected")
		return
	self.currently_selected_gizmo.translation.x = self.player_position.x
	self.currently_selected_gizmo.translation.z = -self.player_position.z


func _on_YSnapToPlayer_pressed():
	if self.currently_selected_gizmo == null:
		print("Warning: No Point Selected")
		return
	self.currently_selected_gizmo.translation.y = self.player_position.y


func _on_SnapSelectedToPlayer_pressed():
	if self.currently_selected_gizmo == null:
		print("Warning: No Point Selected")
		return
	self.currently_selected_gizmo.translation.x = self.player_position.x
	self.currently_selected_gizmo.translation.z = -self.player_position.z
	self.currently_selected_gizmo.translation.y = self.player_position.y


func _on_SetActiveTrail_pressed():
	self.currently_active_guildpoint_trail = self.currently_selected_guildpoint_trail
	self.currently_active_trail3d = self.currently_selected_trail3d
	self.currently_active_trail2d = self.currently_selected_trail2d


func _on_ReverseTrailDirection_pressed():
	if self.currently_selected_guildpoint_trail != null:
		reverse_trail(self.currently_selected_guildpoint_trail, self.currently_selected_trail3d, self.currently_active_trail2d)
	on_gizmo_deselected()
	clear_adjustment_nodes()
	gen_adjustment_nodes()


func _on_ExitButton_pressed():
	if not read_hash(self.map_id) == make_hash(self.guildpoint_data.to_bytes()):
		save_map_data(self.map_id)
	exit_burrito()


func _on_Settings_pressed():
	var settings_dialog: WindowDialog = $Control/Dialogs/SettingsDialog
	settings_dialog.load_settings()
	settings_dialog.show()


func _on_MarkersUI_cell_selected():
	var category_item = self.markers_ui.get_selected()
	self.currently_active_category = category_item
	self.currently_active_trail2d = null
	self.currently_active_trail3d = null
	self.currently_active_guildpoint_trail = null
	on_gizmo_deselected()
	clear_adjustment_nodes()


func _on_MarkersUI_item_edited():
	var category_item = self.markers_ui.get_edited()
	apply_category_visibility_to_nodes(category_item)


func _on_ImportTaco_pressed():
	$Control/Dialogs/ImportPackDialogs/ImportTacoPackDialog.show()

func _on_ImportBurrito_pressed():
	$Control/Dialogs/ImportPackDialogs/ImportBurritoPackDialog.show()

enum MarkerPackType {
	XML,
	GUILDPOINT,
}

func _on_ImportBurritoPackDialog_dir_selected(dir: String):
	import_marker_pack(dir, MarkerPackType.GUILDPOINT)

func _on_ImportBurritoPackDialog_file_selected(path: String):
	import_marker_pack(path, MarkerPackType.GUILDPOINT)

func _on_ImportTacoPackDialog_dir_selected(dir: String):
	import_marker_pack(dir, MarkerPackType.XML)

func _on_ImportTacoPackDialog_file_selected(path: String):
	import_marker_pack(path, MarkerPackType.XML)

func import_marker_pack(dir: String, type):
	var args: PoolStringArray
	if type == MarkerPackType.XML:
		args.push_back("--input-taco-path")
	elif type == MarkerPackType.GUILDPOINT:
		args.push_back("--input-guildpoint-path")
	args.push_back(dir)
	args.append_array([
		"--input-guildpoint-path", ProjectSettings.globalize_path(Settings.get_saved_markers_dir()),
		"--output-guildpoint-path", ProjectSettings.globalize_path(Settings.get_saved_markers_dir()),
		"--split-by-category",
		"--output-guildpoint-path", ProjectSettings.globalize_path(Settings.get_unsaved_markers_dir()),
		"--split-by-map-id",
	])

	var duplicate_categories: Dictionary = FileHandler.call_burrito_converter(args)
	if duplicate_categories.empty():
		save_hashes()
		load_guildpoint_markers(self.map_id)
		return

	var category_names: PoolStringArray
	for id in duplicate_categories.keys():
		for category_name in duplicate_categories[id].keys():
			for file in duplicate_categories[id][category_name]:
				if file.begins_with(ProjectSettings.globalize_path(Settings.get_saved_markers_dir())):
					if !(category_name in category_names):
						category_names.push_back(category_name)
	self.overwrite_confirmation.dialog_text = "The following marker packs will be overwritten. \n" + category_names.join("\n")
	self.overwrite_confirmation.popup_centered()

	# Await result of pop up
	var return_val = yield(self.overwrite_confirmation, "confirmation_result")
	if return_val == true:
		for id in duplicate_categories.keys():
			for category in duplicate_categories[id]:
				for file in duplicate_categories[id][category]:
					if file.begins_with(ProjectSettings.globalize_path(Settings.get_saved_markers_dir())):
						FileHandler.delete_file(file)
		duplicate_categories = FileHandler.call_burrito_converter(args)
		if duplicate_categories.empty():
			save_hashes()
			load_guildpoint_markers(self.map_id)
		else:
			push_error("Duplicates could not be resolved.")

func _on_SaveData_pressed():
	var user_data_dir = str(OS.get_user_data_dir())
	var args: PoolStringArray = [
		"--input-guildpoint-path", ProjectSettings.globalize_path(Settings.get_unsaved_markers_dir()),
		"--output-guildpoint-path", ProjectSettings.globalize_path(Settings.get_saved_markers_dir()),
		"--split-by-category",
	]
	FileHandler.call_burrito_converter(args)
