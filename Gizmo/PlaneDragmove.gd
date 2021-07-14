extends KinematicBody

var camera: Camera
var plane: Array

################################################################################
# Get the definition of a line in the format
#
# x = a + d*t
# y = b + e*t
# z = c + f*t
#
# by assuming point_one is t=0 and point_two is t=1
################################################################################
func get_line_formulas(point_one: Vector3, point_two: Vector3) -> Array:


	# p2 = p1 + d
	return [
		Vector2(point_one.x, point_two.x - point_one.x),
		Vector2(point_one.y, point_two.y - point_one.y),
		Vector2(point_one.z, point_two.z - point_one.z)
	]

################################################################################
# Get the definition of a plane in the format
#
# ax + by + cz = d
#
# via magic
################################################################################
func get_plane_from_points(p1: Vector3, p2: Vector3, p3: Vector3) -> Array:
	var a = ((p2.y - p1.y) * (p3.z - p1.z)) - ((p3.y - p1.y) * (p2.z - p1.z))
	var b = ((p2.z - p1.z) * (p3.x - p1.x)) - ((p3.z - p1.z) * (p2.x - p1.x))
	var c = ((p2.x - p1.x) * (p3.y - p1.y)) - ((p3.x - p1.x) * (p2.y - p1.y))
	var d = -((a * p1.x) + (b * p1.y) +  (c * p1.z))
	return [a, b, c, d]

################################################################################
# Get the intersection of the plane and line by plugging them into each other
# x = a + bt
# y = c + dt
# z = e + ft
#
# gx + hy + iz = j
#
#
#
# g(a + bt) + h(c + dt) + i(e + ft) = j
# ga + gbt + hc + hdt + ie + ift = j
# gbt + hdt + ift ga + hc + ie = j
# t(gb + hd + if) + ga + hc + ie = j
# t(gb + hd + if) = j - (ga + hc + ie)
# t = (j - (ga + hc + ie)) / (gb + hd + if)
#
# TODO: boy is this unreadable, maybe we can make it cleaner? Or at least
# more compact compressing all three functions into one that just accepts 5
# vector3s and outputs one.
################################################################################
func get_line_plane_intersection(line, plane) -> Vector3:
	var a = line[0][0]
	var b = line[0][1]
	var c = line[1][0]
	var d = line[1][1]
	var e = line[2][0]
	var f = line[2][1]

	var g = plane[0]
	var h = plane[1]
	var i = plane[2]
	var j = plane[3]

	var t_denominator = (g*b + h*d + i*f)
	if t_denominator == 0:
		return Vector3(INF, INF, INF)
	var t = (j - (g*a + h*c + i*e)) / t_denominator

	return Vector3(a + (b * t), c + (d * t), e + (f * t))


################################################################################
# Find the intersection between a point emitted from the camera based on where
# the mouse is on the screen, and a pre-defined plane.
#
# TODO: Maybe move the global self.camera into an argument for this function
# TODO: Maybe move the global self.plane into an argument for this function
################################################################################
func get_offset_coordinates(event) -> Vector3:
	var start_coordinate = self.camera.project_ray_origin(event.position)
	var direction = self.camera.project_ray_normal(event.position)

	var line = get_line_formulas(start_coordinate, start_coordinate+direction)
	var intersection = get_line_plane_intersection(line, self.plane)

	return intersection
