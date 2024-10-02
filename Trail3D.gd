extends Spatial

const Guildpoint = preload("res://guildpoint.gd")

var texture_path
var color = Color(0.9, 0.1, 0.1)
var guildpoint: Guildpoint.Trail
var category: TreeItem

func refresh_mesh():
	var tmpMesh = Mesh.new()
	var i = 0
	var last_uv: float = 0.0
	var trail_data = self.guildpoint.get_trail_data()
	for point_index in range(trail_data.get_points_x().size()-1):
		var point:Vector3 = Vector3(trail_data.get_points_x()[point_index], trail_data.get_points_y()[point_index], -trail_data.get_points_z()[point_index])
		var next_point:Vector3 = Vector3(trail_data.get_points_x()[point_index+1], trail_data.get_points_y()[point_index+1], -trail_data.get_points_z()[point_index+1])
		# If the line starts or ends at map coordinates (0,0,0), don't draw the line.
		if point == Vector3(0,0,0) or next_point == Vector3(0,0,0):
			continue

		var distance: float = point.distance_to(next_point)
		var normal: Vector3 = (next_point - point).normalized()
		var horizontal_tangent:Vector3 = Vector3(normal.z, 0, -normal.x).normalized()

		normal = Vector3(0,0,0)

		var point1: Vector3 = point + normal/2 - horizontal_tangent/2
		var point2: Vector3 = point + normal/2 + horizontal_tangent/2
		var point3: Vector3 = next_point - normal/2 + horizontal_tangent/2
		var point4: Vector3 = next_point - normal/2 - horizontal_tangent/2

		var vertices = PoolVector3Array()
		var UVs = PoolVector2Array()

		vertices.push_back(point1)
		vertices.push_back(point2)
		vertices.push_back(point3)
		vertices.push_back(point4)

		var next_uv = last_uv + distance

		UVs.push_back(Vector2(1,last_uv))
		UVs.push_back(Vector2(0,last_uv))
		UVs.push_back(Vector2(0,next_uv))
		UVs.push_back(Vector2(1,next_uv))

		last_uv = next_uv
		if last_uv > 100:
			last_uv = last_uv-100

		var st = SurfaceTool.new()
		st.begin(Mesh.PRIMITIVE_TRIANGLE_FAN)

		for v in vertices.size():
			st.add_color(color)
			st.add_uv(UVs[v])
			st.add_vertex(vertices[v])
		st.commit(tmpMesh)
	$MeshInstance.mesh = tmpMesh


func set_texture(texture):
	$MeshInstance.material_override.set_shader_param("texture_albedo", texture)

