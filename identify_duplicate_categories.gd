################################################################################
# starts_with
#
# A helper function to see if a string starts with a paticular value or not.
################################################################################
static func starts_with(value: String, prefix: String):
	if value.length() < prefix.length():
		return false
	return value.substr(0, prefix.length()) == prefix


################################################################################
# get_duplicate_categories
#
# Parses the stdout of the xml_converter and extracts any category colisions if
# there are any that exist.
################################################################################
static func get_duplicate_categories(stdin: String) -> Dictionary:
	var lines: PoolStringArray = stdin.split("\n")
	var capturing: bool = false
	var colisions: Dictionary = {}
	var category = null
	for line in lines:
		if line == null:
			continue
		elif !capturing:
			if line == "The following top level categories were found in more than one pack:":
				capturing = true
		elif capturing:
			if starts_with(line, "        "):
				var file_name = line.substr(8)
				colisions[category].append(file_name)
			elif starts_with(line, "    "):
				var line_without_prefix = line.substr(5)
				var endquote_index = line_without_prefix.find("\"")
				category = line_without_prefix.substr(0, endquote_index)
				colisions[category] = []
			else:
				break
	return colisions
