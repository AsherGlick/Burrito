extends Spatial

var trails3d: Array = []
var icons: Array = []
var subcategories: Array = []


func add_trail3d(trail3d):
	self.add_child(trail3d, true)
	trails3d.push_back(trail3d)

func add_icon(icon):
	self.add_child(icon, true)
	icons.push_back(icon)

func add_subcategory(subcategory):
	self.add_child(subcategory, true)
	subcategories.push_back(subcategory)

func clear_all():
	self.trails3d = []
	self.icons = []
	self.subcategories = []
	for child in self.get_children():
		child.queue_free()

func remove_icon(icon):
	self.icons.remove(icon)
	icon.queue_free()
