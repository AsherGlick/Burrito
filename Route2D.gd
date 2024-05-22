extends Line2D

func add_new_point(position: Vector3, index: int = -1):
	self.add_point(Vector2(position.x, position.z), index)
	
func update_point_poistion(position: Vector3, index: int):
	self.set_point_position(index, Vector2(position.x, position.z))
	
func new_point_after(midpoint, index):
	var start = self.get_point_position(index)
		
	if self.get_point_count() > index+1:
		var end = self.get_point_position(index+1)
		midpoint = ((start-end)/2) + end
	add_point(midpoint, index+1)
