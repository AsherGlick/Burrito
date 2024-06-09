extends Line2D

func on_update_position(position: Vector3, index: int):
	self.set_point_position(index, Vector2(position.x, position.z))
	
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
	var points = self.points
	points.invert()
	self.points = points
