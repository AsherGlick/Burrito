// NOTE: Shader automatically converted from Godot Engine 3.4.4.stable's CanvasItemMaterial.
shader_type canvas_item;
render_mode blend_mix;

// the UV where the selected button's background hits alpha 0
uniform float left_fade_end: hint_range(0.0,1.0) = 0.0;

// The UV where the selected button's background begins to fade
uniform float left_fade_start: hint_range(0.0,1.0) = 0.071;

// Probably the same as left_fade_start, the left edge of the black bar
uniform float left_edge_end: hint_range(0.0,1.0) = 0.034;

// The right edge of the black bar
uniform float left_edge_start: hint_range(0.0,1.0) = 0.071;


uniform float selected_start: hint_range(0.0, 1.0) = 0.074;
uniform float selected_end: hint_range(0.0, 1.0) = 0.131;
// diff = 0.057

// 0.355
// 0.412
// diff = 0.057

// Increment: 0.0562


// 0.355 + 0.074 + x * 5
void fragment() {
	vec4 color = texture(TEXTURE, UV);

	if (UV.y > selected_start && UV.y < selected_end) {
		if (UV.x < left_fade_end) {
			color.a=0.0;
		}
		else if (UV.x < left_fade_start) {
			color.a = (UV.x-left_fade_end) / (left_fade_start-left_fade_end);
		}
	}
	else {
		if (UV.x < left_edge_end) {
			color.a = 0.0;
		}
		else if (UV.x < left_edge_start) {
			color=vec4(0,0,0,1.0-UV.y);
		}
	}

	COLOR=color;
}
