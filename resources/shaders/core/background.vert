#version 440 core

layout (location = 0) in vec2 vertex_position;
layout (location = 1) in vec2 texture_position;
layout (location = 2) in vec3 color;

layout (std140, binding = 0) uniform CommonData {
	mat4 projection_;
	vec2 set_position_;
};

out FS_DATA
{
	vec2 texture_position;
	vec3 color;
} fs_data;

void main()
{
	gl_Position = projection_ * vec4(vertex_position, 0.0f, 1.0f);

	fs_data.texture_position = texture_position;
	fs_data.color = color;
}
