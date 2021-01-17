#version 440 core

layout (location = 0) in float texture_layer;
layout (location = 1) in vec2 offset;
layout (location = 2) in vec2 scale;
layout (location = 3) in vec4 foreground_color;
layout (location = 4) in vec4 background_color;

uniform mat4 projection;
uniform vec2 origin;

out GS_DATA
{
	float texture_layer;
	vec2 scale;
	vec4 foreground_color;
	vec4 background_color;
} gs_data;

void main()
{
	gl_Position = projection * vec4(origin + offset, 0.0f, 1.0f);

	gs_data.texture_layer = texture_layer;
	gs_data.scale = scale;
	gs_data.foreground_color = foreground_color;
	gs_data.background_color = background_color;
}
