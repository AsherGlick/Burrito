shader_type spatial;
render_mode blend_mix,depth_draw_opaque,cull_back,diffuse_burley,specular_schlick_ggx, unshaded;
uniform vec4 line_color: hint_color;
uniform float line_width = 1;

//uniform bool d_130 = false;
//uniform bool d_150 = false;
//uniform bool d_600 = false;
//uniform bool d_900 = false;
//uniform bool d_1200 = false;
//uniform bool d_1500 = false;
//uniform bool d_custom = false;
//uniform float custom = 600.0;
uniform float transparency = 1.0;

uniform bool custom_1;
uniform float custom_1_value;
uniform bool custom_2;
uniform float custom_2_value;
uniform bool custom_3;
uniform float custom_3_value;
uniform bool custom_4;
uniform float custom_4_value;
uniform bool custom_5;
uniform float custom_5_value;
uniform bool custom_6;
uniform float custom_6_value;
uniform bool custom_7;
uniform float custom_7_value;


bool range(float range, float squared_distance, float modified_line_width){
	float max_squared_distance = ((range+modified_line_width/2.0)) * ((range+modified_line_width/2.0));
	float min_squared_distance = ((range-modified_line_width/2.0)) * ((range-modified_line_width/2.0));
	if (max_squared_distance > squared_distance && min_squared_distance < squared_distance) {
		return true;
	}
	return false;
}

// COLOR.r being used here for distance to camera
void vertex() {
	vec4 view_pos = MODELVIEW_MATRIX * vec4(VERTEX, 1.0);
	float dist = -view_pos.z;
	COLOR.r = dist;
}
const float PI = 3.14159265358979323846;
void fragment() {
	float distance_to_camera = COLOR.r;
	float angle_of_camera = dot(normalize(NORMAL), normalize(VIEW));
	float angle_size_multiplier = 1.0/angle_of_camera;
	
	float squared_distance = ((UV.x-0.5)*3000.0)*((UV.x-0.5)*3000.0) + ((UV.y-0.5)*3000.0)*((UV.y-0.5)*3000.0);
	ALPHA = 0.0;
	ALBEDO = vec3(0,0,0);
	
	
	if (custom_1) {
		if (range(custom_1_value,squared_distance, line_width*distance_to_camera*angle_size_multiplier)) {
			ALBEDO = vec3(1,1,1);
			ALPHA = transparency;
		}
	}
	if (custom_2) {
		if (range(custom_2_value,squared_distance, line_width*distance_to_camera*angle_size_multiplier)) {
			ALBEDO = vec3(1,1,1);
			ALPHA = transparency;
		}
	}
	if (custom_3) {
		if (range(custom_3_value,squared_distance, line_width*distance_to_camera*angle_size_multiplier)) {
			ALBEDO = vec3(1,1,1);
			ALPHA = transparency;
		}
	}
	if (custom_4) {
		if (range(custom_4_value,squared_distance, line_width*distance_to_camera*angle_size_multiplier)) {
			ALBEDO = vec3(1,1,1);
			ALPHA = transparency;
		}
	}
	if (custom_5) {
		if (range(custom_5_value,squared_distance, line_width*distance_to_camera*angle_size_multiplier)) {
			ALBEDO = vec3(1,1,1);
			ALPHA = transparency;
		}
	}
	if (custom_6) {
		if (range(custom_6_value,squared_distance, line_width*distance_to_camera*angle_size_multiplier)) {
			ALBEDO = vec3(1,1,1);
			ALPHA = transparency;
		}
	}
	if (custom_7) {
		if (range(custom_7_value,squared_distance, line_width*distance_to_camera*angle_size_multiplier)) {
			ALBEDO = vec3(1,1,1);
			ALPHA = transparency;
		}
	}
	
	
//	if (d_600) {
//		if (range(600, squared_distance, line_width*distance_to_camera*angle_size_multiplier)) {
//			ALBEDO = vec3(1,1,1);
//			ALPHA = transparency;
//		}
//	}
//	if (d_custom) {
//		if (range(custom, squared_distance, line_width*distance_to_camera*angle_size_multiplier)) {
//			ALBEDO = vec3(1,1,1);
//			ALPHA = transparency;
//		}
//	}
}
