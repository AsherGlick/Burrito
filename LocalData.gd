extends Node

const LOCAL_DATA_PATH = "user://localdata.json"

var visible_dict = {}
var _local_data 

func _ready():
	var file = File.new()
	file.open(LOCAL_DATA_PATH, file.READ)
	var text = file.get_as_text()
	var datum = JSON.parse(text)	
	self._local_data = JSON.parse(text).result
	load_local()
	
func load_local():	
	for key in self._local_data["visible_dict"].keys():
		self.visible_dict[key] = self._local_data["visible_dict"][key]

func save_local():
	_local_data = {
		"visible_dict": visible_dict
	}
		
	var file = File.new()
	file.open(LOCAL_DATA_PATH, File.WRITE)
	file.store_string(JSON.print(self._local_data))
