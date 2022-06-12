#version 440 core

out vec4 fragment_color;

uniform usampler2D font_texture;

in FS_DATA
{
	vec2 texture_position;
	float has_texture;
	vec3 color;
} fs_data;

void main()
{
	if (fs_data.has_texture > 0.0) {
		float sampled = texture(font_texture, fs_data.texture_position).r;

		fragment_color = vec4(fs_data.color, sampled);
	}
	else {
		fragment_color = vec4(fs_data.color, 1.0);
	}
}
