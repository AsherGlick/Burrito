tool
extends Control

#export var button_0_icon: Texture = null setget button_0_icon_setget
#export var button_0_hover: Texture = null setget button_0_hover_setget
#export var button_0_text: String = "" setget button_0_text_setget
#export var button_1_icon: Texture = null setget button_1_icon_setget
export var selected_icon: int = 0 setget selected_icon_setget

func selected_icon_setget(new):
	selected_icon = new
	update_selected()

export(Array, Texture) var button_textures setget button_textures_setget
export(Array, Texture) var button_hover_textures setget button_hover_textures_setget
export(Array, String) var button_text setget button_text_setget

func button_textures_setget(new):
	print("Button Textures Setget")
	button_textures=new
	icon_data_change()
func button_hover_textures_setget(new):
	button_hover_textures = new
	icon_data_change()
func button_text_setget(new):
	button_text = new
	icon_data_change()


const button_top_offset = 74
const button_sequential_offset = 44
const button_left_offset = 1
const button_width = 41
const button_height = 41


func _ready():
	print("Ready")
	icon_data_change()

func _process(delta):
	if dragging:
		var mouse_location = get_viewport().get_mouse_position()
		self.rect_position = mouse_location - origin

################################################################################
# get_default
#
# A helper function to get a value from an array at an index that might not
# exist in the array. If the index does not exist then the default is returned
# instead.
################################################################################
func get_default(array: Array, index: int, default):
	if (index < 0):
		return default
	if (index >= array.size()):
		return default
	return array[index]

################################################################################
# icon_data_change
#
# A function to re-render all of the left icon buttons based on which ones are
# present in the export variables. This recreates the entire button nodes.
################################################################################
func icon_data_change():
	var buttons_elem: Control = $Buttons

	# Exit early if buttons_elem is not initalized
	if buttons_elem == null:
		return
	print("Setting Up Nodes")

	for child in buttons_elem.get_children():
		child.queue_free()

	var max_size = [
		button_textures.size(),
		button_hover_textures.size(),
		button_text.size()
	].max()

	# A blank texture to use as the default
	var default_texture: Texture

	for i in max_size:

		var button_texture: Texture = get_default(button_textures, i, default_texture)
		var button_hover_texture: Texture = get_default(button_hover_textures, i, default_texture)

		var button := TextureButton.new()
		# Make sure the button image is centered
		button.expand = true
		button.stretch_mode = TextureButton.STRETCH_KEEP_CENTERED

		button.texture_normal = button_texture
		button.texture_hover = button_hover_texture
		button.rect_position = Vector2(button_left_offset, button_top_offset + button_sequential_offset*i)
		button.rect_size = Vector2(button_width, button_height)

		buttons_elem.add_child(
			button
		)

		button.connect("pressed", self, "_menu_button_press", [i])

################################################################################
# _menu_button_press
#
# whenever one of the lefthand generated menu buttons is clicked by the user
# this function is called.
################################################################################
func _menu_button_press(index):
	self.selected_icon = index


func update_selected():
	var background_elem: TextureRect = $Background
	var selection_elem: TextureRect = $Selection
	var divider_elem: TextureRect = $Divider
	var subtitle_elem: RichTextLabel = $SubTitle


	# Exit early if at least one of the nodes is not initialized
	if (
		background_elem == null
		|| selection_elem == null
		|| divider_elem == null
		|| subtitle_elem == null
	):
		return

	background_elem.material.set_shader_param("selected_start", 0.074 + 0.0562 * selected_icon)
	background_elem.material.set_shader_param("selected_end", 0.131 + 0.0562 * selected_icon)

	divider_elem.material.set_shader_param("selected_start", 0.074 + 0.0562 * selected_icon)
	divider_elem.material.set_shader_param("selected_end", 0.131 + 0.0562 * selected_icon)

	selection_elem.rect_position.y = 44 + 44 * selected_icon

	subtitle_elem.text = get_default(button_text, selected_icon, "")

################################################################################
# Dragging start/stop logic
# the actual movement logic of the window dragging is handled in _process
var dragging = false
var origin: Vector2 = Vector2(0, 0)
# Start Dragging
func _on_TitleBar_button_down():
	dragging = true
	origin = get_viewport().get_mouse_position() - self.rect_position
# Stop Dragging
func _on_TitleBar_button_up():
	dragging = false

# Hide the window
func _on_CloseButton_pressed():
	self.hide()
