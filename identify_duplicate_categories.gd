################################################################################
# get_duplicate_categories
#
# Parses the stdout of the xml_converter and extracts any category collisions
# if there are any that exist.
#
# Example Input
# =============
#   Did not write due to duplicates in categories.
#   This commonly occurs when attempting to read the same pack multiple times or when separate packs coincidentally have the same name.
#   Please remove one of the packs or edit the name of the packs' top level category before running the program again.
#   If you want to bypass this stop, use '--allow-duplicates'.
#   The following top level categories were found in more than one pack:
#       "mycategory" in files:
#           pack/xml_file.xml
#           pack2/markers.guildpoint
#           pack3/xml_file.xml
#
# Example Output
# ==============
#   {
#     "mycategory": [
#       "pack/xml_file.xml",
#       "pack2/markers.guildpoint",
#       "pack3/xml_file.xml",
#     ]
#   }
################################################################################
static func get_duplicate_categories(stdin: String) -> Dictionary:
	var lines: PoolStringArray = stdin.split("\n")
	var capturing: bool = false
	var collisions: Dictionary = {}
	var category = null
	for line in lines:
		if line == null:
			continue
		elif !capturing:
			if line == "The following top level categories were found in more than one pack:":
				capturing = true
		elif capturing:
			if line.begins_with("        "):
				# Remove the first 8 spaces
				var file_name = line.substr(8)
				collisions[category].append(file_name)
			elif line.begins_with("    "):
				# Remove the first 4 spaces and the open quote
				var line_without_prefix = line.substr(5)
				var endquote_index = line_without_prefix.find("\"")
				category = line_without_prefix.substr(0, endquote_index)
				collisions[category] = []
			else:
				break
	return collisions
