extends MeshInstance


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

export var recover_time:float = .1
var velocity:float = 0
var target_location: Vector3 = Vector3(0,0,0);
var target_direction:Vector3 = Vector3(1,0,0);
var is_animating: bool = false


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	
	if self.is_animating:
		if (self.target_location - self.translation).length() < (self.velocity*delta) or self.velocity < 0:
			self.translation = self.target_location
			self.is_animating = false
			self.velocity = 0
		else:
			self.translation += self.target_direction * self.velocity*delta


func moveto(location: Vector3):
	self.target_location = location
	var direction = (self.target_location - self.translation)
	self.target_direction = direction.normalized()
	var new_velocity = direction.length() / recover_time
	if new_velocity > self.velocity:
		self.velocity = new_velocity
	self.is_animating = true
