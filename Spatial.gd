extends Spatial

var server := UDPServer.new()
var peers = []

# Called when the node enters the scene tree for the first time.
func _ready():
	get_tree().get_root().set_transparent_background(true)
	#ProjectSettings.set_setting("display/window/size/always_on_top", true)
	#OS.set_window_always_on_top(true)
	
	OS.window_maximized = false
	OS.window_size = Vector2(1920, 1080)
	OS.window_position = Vector2(0, 0) # TODO: This does not seem to work
	
	var clickthrough: PoolVector2Array = [Vector2(100,100),Vector2(100,200),Vector2(200,200),Vector2(200,100)]
	
	OS.set_window_mouse_passthrough(clickthrough)
	
	server.listen(4242)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	#$MeshInstance.rotate(Vector3(0,1,0), .5 * delta)
	#$MeshInstance.rotate_object_local(Vector3(1,0,0), 1 * delta)
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
	var textbox_has_focus: bool = (ui_flags & 0x20) == 0x20;
	var in_combat: bool = (ui_flags & 0x40) == 0x40;
	
	# Position the camera in the same location as it in in-game
	$CameraMount.translation.x = camera_position.x
	$CameraMount.translation.y = camera_position.y
	$CameraMount.translation.z = -camera_position.z
	
	# Orent the camera in the same rotation as it is facing in game
	$CameraMount/Camera.rotation.x = asin(camera_facing.y)
	$CameraMount.rotation.y = -atan2(camera_facing.x, camera_facing.z)
	#print(-atan2(camera_facing.x, camera_facing.z))

#	$MeshInstance.translation.x = player_position.x
#	$MeshInstance.translation.y = player_position.y
#	$MeshInstance.translation.z = -player_position.z

#	print(map_is_open, map_rotation, map_offset)
func decode_context_packet(spb: StreamPeerBuffer):
	var compas_width: int = spb.get_16()
	var compass_height: int = spb.get_16()
	var map_id: int = spb.get_32()
	
	var identity_length: int = spb.get_32()
	var identity_str = spb.get_utf8_string(identity_length)
	var identity = JSON.parse(identity_str).result
	
	# FOV Calculations
	# The minimum value on the FOV slider gives a float value in this field of 0.436
	# The maximum value on the FOV slider gives a float value in this field of 1.222
	# According to https://thatshaman.com/tools/fov/ the FOV range is (25 -> 70)
	# This function maps a value between 0.436 and 1.222 to a value between 25 and 70
	$CameraMount/Camera.fov = (((identity["fov"] - 0.436) / 0.786) * 45) + 25
