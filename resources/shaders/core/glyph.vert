#version 440 core

layout (location = 0) in float texture_layer;
layout (location = 1) in vec2 position;
layout (location = 2) in vec3 foreground_color;
layout (location = 3) in vec3 background_color;

layout (std140, binding = 0) uniform CommonData {
	mat4 projection_;
	vec2 font_size_;
	vec2 set_position_;
	vec2 set_padding_;
	vec2 set_spacing_;
};

out GS_DATA
{
	float texture_layer;
	vec3 foreground_color;
	vec3 background_color;
} gs_data;

void main()
{
	gl_Position = projection_ * vec4(set_position_ + (position * font_size_), 0.0f, 1.0f);

	gs_data.texture_layer = texture_layer;
	gs_data.foreground_color = foreground_color;
	gs_data.background_color = background_color;
}
