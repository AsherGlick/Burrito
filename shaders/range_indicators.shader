shader_type spatial;
render_mode blend_mix,depth_draw_opaque,cull_back,diffuse_burley,specular_schlick_ggx;
uniform vec4 line_color: hint_color;
uniform float line_width = 1;

uniform bool d_130 = false;
uniform bool d_150 = false;
uniform bool d_600 = false;
uniform bool d_900 = false;
uniform bool d_1200 = false;
uniform bool d_1500 = false;
uniform bool d_custom = false;
uniform float custom = 600.0;
uniform float transparency = 1.0;

bool range(float range, float squared_distance){
	float max_squared_distance = ((range+line_width/2.0)) * ((range+line_width/2.0));
	float min_squared_distance = ((range-line_width/2.0)) * ((range-line_width/2.0));
	if (max_squared_distance > squared_distance && min_squared_distance < squared_distance) {
		return true;
	}
	return false;
}

void fragment() {
	float squared_distance = ((UV.x-0.5)*3000.0)*((UV.x-0.5)*3000.0) + ((UV.y-0.5)*3000.0)*((UV.y-0.5)*3000.0);
	ALPHA = 0.0;
	ALBEDO = vec3(0,0,0);
//
	if (d_600) {
		if (range(600, squared_distance)) {
			ALBEDO = vec3(1,1,1);
			ALPHA = transparency;
		}
	}
	if (d_custom) {
		if (range(custom, squared_distance)) {
			ALBEDO = vec3(1,1,1);
			ALPHA = transparency;
		}
	}
}
