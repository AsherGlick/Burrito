extends Spatial

var trails: Array = []
var icons: Array = []
var subcategories: Array = []


func add_trail(trail):
	self.add_child(trail, true)
	trails.push_back(trail)

func add_icon(icon):
	self.add_child(icon, true)
	icons.push_back(icon)

func add_subcategory(subcategory):
	self.add_child(subcategory, true)
	subcategories.push_back(subcategory)

func clear_all():
	self.trails = []
	self.icons = []
	self.subcategories = []
	for child in self.get_children():
		child.queue_free()

func remove_icon(icon):
	self.icons.remove(icon)
	icon.queue_free()
