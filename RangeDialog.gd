extends WindowDialog


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

var distance_indicator: MeshInstance
# Called when the node enters the scene tree for the first time.
func _ready():
	self.distance_indicator = get_tree().get_root().get_node("Spatial/FeetLocation/DistanceIndicator")

func on_change(value=null):
	var index = 1
	var all_disabled = true
	var should_enable_next = false;
	for child in $GridContainer.get_children():
		if child is CheckButton:
			should_enable_next = child.pressed
			self.distance_indicator.get_surface_material(0).set_shader_param("custom_"+str(index), child.pressed)
		elif child is SpinBox:
			child.editable = should_enable_next
			self.distance_indicator.get_surface_material(0).set_shader_param("custom_"+str(index)+"_value", child.value)
			index+=1
			if should_enable_next:
				all_disabled = false
		else:
			print(child)
	if all_disabled:
		self.distance_indicator.hide()
	else:
		self.distance_indicator.show()
