# Clean all the dialog prompt paths to not include local dev data
sed --in-place -e 's/current_dir = "[^"]*"/current_dir = "."/g' *.tscn
sed --in-place -e 's/current_file = "[^"]*"/current_file = "."/g' *.tscn
sed --in-place -e 's/current_path = "[^"]*"/current_path = "."/g' *.tscn
