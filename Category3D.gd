extends Spatial

var paths: Array = []
var icons: Array = []
var subcategories: Array = []


func add_path(path):
	self.add_child(path, true)
	paths.push_back(path)

func add_icon(icon):
	self.add_child(icon, true)
	icons.push_back(icon)

func add_subcategory(subcategory):
	self.add_child(subcategory, true)
	subcategories.push_back(subcategory)

func clear_all():
	self.paths = []
	self.icons = []
	self.subcategories = []
	for child in self.get_children():
		child.queue_free()

func remove_icon(icon_index):
	var icon = self.icons[icon_index]
	self.icons.remove(icon)
	icon.queue_free()
