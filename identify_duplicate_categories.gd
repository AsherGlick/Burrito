################################################################################
# get_duplicate_categories
#
# Parses the stdout of burrito_converter and extracts any category collisions
# if there are any that exist.
#
# Example Input
# =============
#   Did not write due to duplicates in categories.
#   This commonly occurs when attempting to read the same pack multiple times or when separate packs coincidentally have the same name.
#   Please remove one of the packs or edit the name of the packs' top level category before running the program again.
#   If you want to bypass this stop, use '--allow-duplicates'.
#   The following top level categories had a conflict in IDs. For XML, these IDs may be generated from the 'name' attribute.
#       Categories were found that share the ID "92RAfU6hdh4="
#           In these files the Category 'name' is "mc"
#               pack/xml_file.xml
#               pack3/xml_file.xml
#           In these files the Category 'name' is "mycategory"
#               pack2/markers.guildpoint
#
# Example Output
# ==============
#   {
#       "ID": {
#         "mc": [
#            "pack/xml_file.xml",
#            "pack3/xml_file.xml",
#          ],
#         "mycategory": [
#            "pack2/markers.guildpoint",
#          ]
#       }
#   }
################################################################################
static func get_duplicate_categories(stdin: String) -> Dictionary:
	var lines: PoolStringArray = stdin.split("\n")
	var capturing: bool = false
	var collisions: Dictionary = {}
	var category = null
	var id = null
	for line in lines:
		if line == null:
			continue
		elif !capturing:
			if line == "The following top level categories had a conflict in IDs. For XML, these IDs may be generated from the 'name' attribute.":
				capturing = true
		elif capturing:
			if line.begins_with("    Categories"):
				# Remove the first 44 characters from the message and the open quote
				var line_without_prefix = line.substr(45)
				var endquote_index = line_without_prefix.find("\"")
				id = line_without_prefix.substr(0, endquote_index)
				collisions[id] = {}
			elif line.begins_with("        In these files no name was found"):
				collisions[id][""] = []
			elif line.begins_with("            "):
				# Remove the first 12 spaces
				var file_name = line.substr(12)
				collisions[id][category].append(file_name)
			elif line.begins_with("        In these files the Category 'name' is "):
				# Remove the first 46 characters and the open quote
				var line_without_prefix = line.substr(47)
				var endquote_index = line_without_prefix.find("\"")
				category = line_without_prefix.substr(0, endquote_index)
				collisions[id][category] = []
			else:
				break
	return collisions
