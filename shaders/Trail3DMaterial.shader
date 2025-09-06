shader_type spatial;
//render_mode blend_mix,depth_draw_opaque,cull_back,diffuse_burley,specular_schlick_ggx;
render_mode unshaded, blend_mix, depth_draw_opaque, cull_disabled, diffuse_burley, specular_schlick_ggx;

uniform sampler2D texture_albedo : hint_albedo; // TODO: Make instance uniform sampler2D when 4.0 is released
uniform vec2 map_size;
uniform bool map_flipped;
uniform float map_bottom_offset = 36.0;

uniform float interval = 1.0;

uniform vec3 cutout_data;

void vertex() {}




void fragment() {
	if (SCREEN_UV.x * VIEWPORT_SIZE.x > VIEWPORT_SIZE.x - map_size.x) {
		if (map_flipped && SCREEN_UV.y * VIEWPORT_SIZE.y > VIEWPORT_SIZE.y - map_size.y) {
			return;
		}
		if (!map_flipped && SCREEN_UV.y * VIEWPORT_SIZE.y < map_size.y + map_bottom_offset) {
			return;
		}
	}

	if (
		abs(SCREEN_UV.x - 0.5)*VIEWPORT_SIZE.x < cutout_data.z
		&& SCREEN_UV.y * VIEWPORT_SIZE.y > cutout_data.x
		&& SCREEN_UV.y * VIEWPORT_SIZE.y < cutout_data.y
	) {
		return;
	}

	vec2 base_uv = vec2(UV.y, -UV.x * interval);
	base_uv = vec2(UV.x, -UV.y);
	vec4 albedo_tex = texture(texture_albedo,base_uv);
	ALBEDO = albedo_tex.rgb;
	ALPHA = albedo_tex.a;
	//EMISSION = albedo_tex.rgb;
	//METALLIC = 0.0;
	//ROUGHNESS = 1.0;
	//SPECULAR = 0.0;
}
