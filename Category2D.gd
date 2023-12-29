extends Node2D

var paths2d: Array = []
var subcategories: Array = []

func add_path2d(path):
	self.add_child(path, true)
	paths2d.push_back(path)

func add_subcategory(subcategory):
	self.add_child(subcategory, true)
	subcategories.push_back(subcategory)
