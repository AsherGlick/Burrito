extends Spatial

var server := UDPServer.new()
var peers = []

var map_id:int = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	get_tree().get_root().set_transparent_background(true)
	#ProjectSettings.set_setting("display/window/size/always_on_top", true)
	#OS.set_window_always_on_top(true)

	OS.window_maximized = false
	OS.window_size = Vector2(1920, 1080)
	set_minimal_mouse_block()
	server.listen(4242)

	# load_taco_markers("/home/vault/Downloads/tw_ALL_IN_ONE/tw_mc_coretyria.xml")

func set_minimal_mouse_block():
	var clickthrough: PoolVector2Array = [
		Vector2(287,0),
		Vector2(314,0),
		Vector2(314,32),
		Vector2(287,32)
	]
	OS.set_window_mouse_passthrough(clickthrough)

func set_maximal_mouse_block():
	OS.set_window_mouse_passthrough([])

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	#OS.window_position = Vector2(1920, 0) # TODO: This does not seem to work	
	#OS.set_window_position(Vector2(1920,0))
	#print(OS.window_position)
	
	#$MeshInstance.rotate(Vector3(0,1,0), .5 * delta)
	#$MeshInstance.rotate_object_local(Vector3(1,0,0), 1 * delta)
	# print(delta)
	server.poll() # Important
	if server.is_connection_available():
		var peer: PacketPeerUDP = server.take_connection()
#		var pkt = peer.get_packet()
		print("accepted peer: %s:%s" % [peer.get_packet_ip(), peer.get_packet_port()])
#		print("Received data: %s" % [pkt.get_string_from_utf8()])
		
#		print(peer.is_listening())

		#peer.put_packet(pkt)
		peers.append(peer)
#		print(len(peers))
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
	var camera_position = Vector3(
		spb.get_float(),
		spb.get_float(),
		spb.get_float()
	)
	var camera_facing = Vector3(
		spb.get_float(),
		spb.get_float(),
		spb.get_float()
	)
	var player_position = Vector3( # Foot Position
		spb.get_float(),
		spb.get_float(),
		spb.get_float()
	)
	var map_offset = Vector2(
		spb.get_float(),
		spb.get_float()
	)
	var map_scale: float = spb.get_float()
	var map_rotation: float = spb.get_float()
	var ui_flags: int = spb.get_32()
	
	var map_is_open: bool = (ui_flags & 0x01) == 0x01;
	var compass_is_top_right: bool = (ui_flags & 0x02) == 0x02;
	var compass_rotation_is_enabled: bool = (ui_flags & 0x04) == 0x04;
	var game_is_focused: bool = (ui_flags & 0x08) == 0x08;
	var in_competitive_game_mode: bool = (ui_flags & 0x10) == 0x10;
	var chatbox_has_focus: bool = (ui_flags & 0x20) == 0x20;
	var in_combat: bool = (ui_flags & 0x40) == 0x40;
	var unchecked_flag = (ui_flags & 0xFFFFFF80) != 0x00000000;

	$PathsAndIcons.visible = !map_is_open

	if unchecked_flag:
		print("Unchecked Flag", (ui_flags & 0xFFFFFF80))
	
#	if game_is_focused:
#		print("game_is_focused")
#	else:
#		print("game is not focused")

#	print("chatbox has focus ", chatbox_has_focus)
	
	# Position the camera in the same location as it in in-game
	$CameraMount.translation.x = camera_position.x
	$CameraMount.translation.y = camera_position.y
	$CameraMount.translation.z = -camera_position.z
	
	# Orent the camera in the same rotation as it is facing in game
	$CameraMount/Camera.rotation.x = asin(camera_facing.y)
	$CameraMount.rotation.y = -atan2(camera_facing.x, camera_facing.z)
	#print(-atan2(camera_facing.x, camera_facing.z))
	
	#print(player_position.y)

#	$MeshInstance.translation.x = player_position.x
#	$MeshInstance.translation.y = player_position.y
#	$MeshInstance.translation.z = -player_position.z

#	print(map_is_open, map_rotation, map_offset)
func decode_context_packet(spb: StreamPeerBuffer):
	var compas_width: int = spb.get_16()
	var compass_height: int = spb.get_16()
	self.map_id = spb.get_32()
	
	var identity_length: int = spb.get_32()
	var identity_str = spb.get_utf8_string(identity_length)
	var identity = JSON.parse(identity_str).result
	
	# FOV Calculations
	# The minimum value on the FOV slider gives a float value in this field of 0.436
	# The maximum value on the FOV slider gives a float value in this field of 1.222
	# According to https://thatshaman.com/tools/fov/ the FOV range is (25 -> 70)
	# This function maps a value between 0.436 and 1.222 to a value between 25 and 70
	$CameraMount/Camera.fov = (((identity["fov"] - 0.436) / 0.786) * 45) + 25

var markerdata = {}
var marker_file_path = ""
func load_taco_markers(marker_json_file):
	self.marker_file_path = marker_json_file
	print("Loading Path", marker_json_file)
	
	var file = File.new()
	file.open(marker_json_file, file.READ)
	var text = file.get_as_text()
	self.markerdata = JSON.parse(text).result
	gen_map_markers()

var path_scene = load("res://Path.tscn")
var icon_scene = load("res://Icon.tscn")


func gen_map_markers():
	var paths_and_icons = $PathsAndIcons

	if str(map_id) in markerdata:
		print("Found Map")
		var map_markerdata = markerdata[str(map_id)]
		print("Paths Found:", len(map_markerdata["paths"]))
		for path in map_markerdata["paths"]:
			#print("Path", path["texture"])
			var new_path = path_scene.instance()
			var new_curve = Curve3D.new()
			for point in path["points"]:
				new_curve.add_point(Vector3(point[0], point[1], -point[2]))
			print(new_curve.get_point_count())
			new_path.curve = new_curve

			paths_and_icons.add_child(new_path)
			
			var texture_file = File.new()
			var image = Image.new()
			texture_file.open(self.marker_file_path.get_base_dir() + "/"+ path["texture"], File.READ)
			image.load_png_from_buffer(texture_file.get_buffer(texture_file.get_len()))
			texture_file.close()
			image.lock()
			
			var texture = ImageTexture.new()
			texture.create_from_image(image, 6)
#			var texture = Image.new()
#			texture.load(self.marker_file_path.get_base_dir() + "/"+ path["texture"])
#			print(texture)
			new_path.get_node("CSGPolygon").material.set_shader_param("texture_albedo", texture)
#
			# Create a new path instance
		for icon in map_markerdata["icons"]:
			var new_icon = icon_scene.instance()
			new_icon.translation = Vector3(icon["position"][0], icon["position"][1], -icon["position"][2])
			
			var texture_file = File.new()
			var image = Image.new()
			texture_file.open(self.marker_file_path.get_base_dir() + "/"+ icon["texture"], File.READ)
			image.load_png_from_buffer(texture_file.get_buffer(texture_file.get_len()))
			texture_file.close()
			image.lock()
			
			var texture = ImageTexture.new()
			texture.create_from_image(image) #, 6)
			new_icon.texture = texture
			
			paths_and_icons.add_child(new_icon)
			#new_icon.translation = Vector3()
			#print("Icon", icon["texture"])
		#	pass

func _on_main_menu_toggle_pressed():
	$Control/FileDialog.show()
	set_maximal_mouse_block()
	pass # Replace with function body.


func _on_FileDialog_hide():
	set_minimal_mouse_block()
	pass # Replace with function body.


func _on_FileDialog_file_selected(path):
	load_taco_markers(path)
	pass # Replace with function body.
