#version 440 core

out vec4 fragment_color;

uniform sampler2D bg_texture;

in FS_DATA
{
	vec2 texture_position;
	vec3 color;
} fs_data;

void main()
{
	fragment_color = texture(bg_texture, fs_data.texture_position) * vec4(fs_data.color, 1.0);
}
