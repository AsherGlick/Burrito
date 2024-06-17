extends Line2D

const Waypoint = preload("res://waypoint.gd")
var waypoint: Waypoint.Trail

func refresh_points():
	var path_points := PoolVector2Array()
	var trail_data = self.waypoint.get_trail_data()
	for index in range(0, trail_data.get_points_z().size()):
		path_points.append(Vector2(trail_data.get_points_x()[index], -trail_data.get_points_z()[index]))
	self.points = path_points
