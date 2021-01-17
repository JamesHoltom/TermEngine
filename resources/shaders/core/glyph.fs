#version 440 core

out vec4 fragment_color;

uniform usampler2DArray font_texture;

in FS_DATA
{
	vec3 texture_position;
	vec4 foreground_color;
	vec4 background_color;
} fs_data;

void main()
{
	if (fs_data.texture_position.z > 0) {
		float sampled = texture(font_texture, fs_data.texture_position).r;

		if (sampled > 0.0f) {
			fragment_color = vec4(1.0f, 1.0f, 1.0f, sampled) * fs_data.foreground_color;
		} else {
			fragment_color = vec4(1.0f, 1.0f, 1.0f, 1.0f - sampled) * fs_data.background_color;
		}
	}
	else {
		fragment_color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
}
