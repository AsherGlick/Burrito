extends Spatial

const Waypoint = preload("res://waypoint.gd")
var waypoint_category: Waypoint.Category

var paths: Array = []
var icons: Array = []
var subcategories: Array = []

func add_path(path):
	self.add_child(path)
	paths.push_back(path)

func add_icon(icon):
	self.add_child(icon)
	icons.push_back(icon)
	
func add_subcategory(subcategory):
	self.add_child(subcategory)
	subcategories.push_back(subcategory)

func _ready():
	pass 
