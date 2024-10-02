extends Line2D

const Guildpoint = preload("res://guildpoint.gd")
var guildpoint: Guildpoint.Trail

func refresh_points():
	var trail_points := PoolVector2Array()
	var trail_data = self.guildpoint.get_trail_data()
	for index in range(0, trail_data.get_points_z().size()):
		trail_points.append(Vector2(trail_data.get_points_x()[index], -trail_data.get_points_z()[index]))
	self.points = trail_points
