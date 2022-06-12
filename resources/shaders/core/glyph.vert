#version 440 core

layout (location = 0) in vec2 position;
layout (location = 1) in float has_texture;
layout (location = 2) in vec2 texture_position;
layout (location = 3) in vec3 color;

layout (std140, binding = 0) uniform CommonData {
	mat4 projection_;
	vec2 font_size_;
	vec2 set_position_;
	vec2 set_padding_;
	vec2 set_spacing_;
};

out FS_DATA
{
	vec2 texture_position;
	float has_texture;
	vec3 color;
} fs_data;

void main()
{
	gl_Position = projection_ * vec4(set_position_ + position, 0.0f, 1.0f);

	fs_data.texture_position = texture_position;
	fs_data.has_texture = has_texture;
	fs_data.color = color;
}
