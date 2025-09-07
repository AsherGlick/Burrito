extends Control


onready var file_list: ItemList = $FileList
onready var directory_path: TextEdit = $DirectoryPath

const icon = preload("res://burrito.png")

func onshow():
	# get the filepath
	directory_path.text = Settings.last_opened_directory
	refresh_item_list(directory_path.text)


func refresh_item_list(path: String):
	print("Refreshing the file list for " + path)
	file_list.clear()

	var dir = Directory.new()
	if dir.open(path) == OK:
		dir.list_dir_begin(true, true)
		var filename = dir.get_next()
		while filename != "":
			
			var filetype = is_file_or_directory(path, filename)
			
			file_list.add_item(filename, icon, true)
			
			file_list.set_item_metadata(file_list.get_item_count()-1, filetype)
			filename = dir.get_next()
		dir.list_dir_end()


# An item in the FileList was double clicked
func _on_FileList_item_activated(index):
	var path = file_list.get_item_text(index)
	var filetype = file_list.get_item_metadata(index)
	
	if filetype == "file":
		pass
	elif filetype == "directory":
		directory_path.text = directory_path.text.plus_file(path)
		# Manually call directorypath changed because the line above does
		# not seem to trigger the signal in godot 3.3.2
		_on_DirectoryPath_text_changed()

	else:
		printerr("unknown filetype found")



func _on_DirectoryPath_text_changed():
	print("Texed Changed")
	Settings.last_opened_directory = directory_path.text
	Settings.save()
	refresh_item_list(directory_path.text)


func is_file_or_directory(path: String, filename: String) -> String:
	var full_path = path.plus_file(filename)

	# First check if it's a file
	var f = File.new()
	if f.file_exists(full_path):
		return "file"

	# Then check if it's a directory
	var d = Directory.new()
	if d.dir_exists(full_path):
		return "directory"

	# If neither exists
	return "none"


func _on_UpDirectory_pressed():
	directory_path.text = directory_path.text.get_base_dir()
	# Manually call directorypath changed because the line above does
	# not seem to trigger the signal in godot 3.3.2
	_on_DirectoryPath_text_changed()
