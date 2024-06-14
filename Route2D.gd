extends Line2D

const Waypoint = preload("res://waypoint.gd")
var waypoint: Waypoint.Trail

func new_point_after(midpoint: Vector3, index: int):
	var midpoint2d: Vector2 = Vector2(midpoint.x, midpoint.z)
	var start: Vector2 = self.get_point_position(index)
		
	if self.get_point_count() > index+1:
		var end: Vector2 = self.get_point_position(index+1)
		midpoint2d = ((start-end)/2) + end
	add_point(midpoint2d, index+1)

func remove_point(index: int):
	self.remove_point(index)

func reverse():
	self.points.invert()
