extends Spatial

var server := UDPServer.new()
var peers = []

# Called when the node enters the scene tree for the first time.
func _ready():
	get_tree().get_root().set_transparent_background(true)
	ProjectSettings.set_setting("display/window/size/always_on_top", true)
	
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
			var x = spb.get_float()
			var y = spb.get_float()
			var z = spb.get_float()
			
			var frontx = spb.get_float()
			var fronty = spb.get_float()
			var frontz = spb.get_float()
			
			var playerx = spb.get_float()
			var playery = spb.get_float()
			var playerz = spb.get_float()
			
			
			$CameraMount.translation.x=x
			$CameraMount.translation.z=-z
			$CameraMount.translation.y=y
			
			$CameraMount/Camera.rotation.x = asin(fronty)
			$CameraMount.rotation.y = -atan2(frontx, frontz)
			#$Camera.rotation.z = frontz
			#print(frontx,"|", fronty,"|", frontz)
			
#			print(.get_string_from_utf8())
		#print()

	return
	
	# Fetch - grabs the data from the shared memory
	# X/Y/Z - grabs xyz coordinates from the cache
	# Pitch/Yaw - grabs the pitch/yaw from the cache
	# Map - grabs the map name from the cache

