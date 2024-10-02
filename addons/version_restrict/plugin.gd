tool
extends EditorPlugin

var confirm_modal: AcceptDialog
var exit_on_hide: bool = true

var target_engine_property = "global/target_engine_version"

################################################################################
# _enter_tree
#
# When the plugin loads read the target_engine_version from the project config
# and compare it to the current engine version.
################################################################################
func _enter_tree():
	var target_engine_version = ProjectSettings.get(target_engine_property)

	var engine_info = Engine.get_version_info()
	var current_engine_version = "{major}.{minor}.{patch}".format({
		"major": engine_info.major,
		"minor": engine_info.minor,
		"patch": engine_info.patch,
	})

	# Set the target version to the current version on first run.
	if target_engine_version == null:
		ProjectSettings.set(target_engine_property, current_engine_version)
		target_engine_version = current_engine_version

	if target_engine_version != current_engine_version:

		confirm_modal = AcceptDialog.new()

		# Remove the `X` button in the top right. We cant remove it compleetly
		# without effecting the stability of the engine so it is just hidden.
		confirm_modal.get_close_button().hide()

		# Rename "Ok" confirmation button to "Close Godot"
		confirm_modal.get_ok().text = "Close Godot"

		get_editor_interface().get_editor_viewport().add_child(confirm_modal)

		confirm_modal.dialog_text = "You are using v"+current_engine_version+" of the Godot Engine to run this project.\n\nThis project wants you to use v"+target_engine_version+" instead.\n\nIf you beleive this is incorrect you can \n- Change the target engin version in:\n    'Project Settings' -> 'Global' -> 'Target Engine Version'\nor\n-  Disable the 'Version Restrict' plugin"

		# Center the panel on the screen
		confirm_modal.anchor_left = 0.5
		confirm_modal.anchor_top = 0.5
		confirm_modal.anchor_right = 0.5
		confirm_modal.anchor_bottom = 0.5
		confirm_modal.margin_left = -confirm_modal.rect_size.x/2
		confirm_modal.margin_top = -confirm_modal.rect_size.y/2

		# Cause the program to exit when the modal is hidden
		confirm_modal.connect("hide", self, "_accept_dialog_hidden")

		# TODO: Add a button to bring you to the download page for the correct version

		confirm_modal.show()


################################################################################
# _accept_dialog_hidden
#
# Triggers when the accept dialog is hidden by any means. Will close the Godot
# editor unless self.exit_on_hide has been set to false somewhere.
################################################################################
func _accept_dialog_hidden():
	if self.exit_on_hide:
		get_tree().quit()


################################################################################
# _exit_tree
#
# If an AcceptDialog was created when this plugin entered the tree then remove
# it. This will prevent the program from exiting when the dialog is closed.
################################################################################
func _exit_tree():
	if confirm_modal:
		self.exit_on_hide = false
		confirm_modal.queue_free()
