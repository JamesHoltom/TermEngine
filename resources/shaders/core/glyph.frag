#version 440 core

out vec4 fragment_color;

uniform usampler2DArray font_texture;

in FS_DATA
{
	vec3 texture_position;
	vec3 foreground_color;
	vec3 background_color;
} fs_data;

void main()
{
	if (fs_data.texture_position.z >= 0) {
		float sampled = texture(font_texture, fs_data.texture_position).r;

		if (sampled > 0.0f) {
			fragment_color = vec4(fs_data.foreground_color, sampled);
		} else {
			fragment_color = vec4(fs_data.background_color, 1.0f - sampled);
		}
	}
	else {
		fragment_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}
