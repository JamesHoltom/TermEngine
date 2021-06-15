#version 440 core

layout (location = 0) in vec2 vert_position;
layout (location = 1) in vec2 size;
layout (location = 2) in vec3 color;

layout (std140, binding = 0) uniform CommonData {
	mat4 projection_;
	vec2 font_size_;
	vec2 set_position_;
	vec2 set_padding_;
	vec2 set_spacing_;
};

out GS_DATA
{
	vec2 size;
	vec3 color;
} gs_data;

void main()
{
	gl_Position = projection_ * vec4(vert_position, 1.0f, 1.0f);

	gs_data.size = size;
	gs_data.color = color;
}
