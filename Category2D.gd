extends Node2D

var trails2d: Array = []
var subcategories: Array = []

func add_trail2d(path):
	self.add_child(path, true)
	trails2d.push_back(path)

func add_subcategory(subcategory):
	self.add_child(subcategory, true)
	subcategories.push_back(subcategory)

func clear_all():
	self.trails2d = []
	self.subcategories = []
	for child in self.get_children():
		child.queue_free()
