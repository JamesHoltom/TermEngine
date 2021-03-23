#version 440 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (std140, binding = 0) uniform CommonData {
	mat4 projection_;
	vec2 font_size_;
	vec2 set_position_;
	vec2 set_padding_;
	vec2 set_spacing_;
};

in GS_DATA
{
	float texture_layer;
	vec4 foreground_color;
	vec4 background_color;
} gs_data[];

out FS_DATA
{
	vec3 texture_position;
	vec4 foreground_color;
	vec4 background_color;
} fs_data;

void draw_quad(vec4 position)
{
	fs_data.foreground_color = gs_data[0].foreground_color;
	fs_data.background_color = gs_data[0].background_color;
	
	// Top-left corner.
	gl_Position = position;
	fs_data.texture_position = vec3(0.0f, 0.0f, gs_data[0].texture_layer);
	EmitVertex();
	
	// Bottom-left corner.
	gl_Position = position + (projection_ * vec4(0.0f, font_size_.y, 0.0f, 0.0f));
	fs_data.texture_position = vec3(0.0f, 1.0f, gs_data[0].texture_layer);
	EmitVertex();
	
	// Top-right corner.
	gl_Position = position + (projection_ * vec4(font_size_.x, 0.0f, 0.0f, 0.0f));
	fs_data.texture_position = vec3(1.0f, 0.0f, gs_data[0].texture_layer);
	EmitVertex();
	
	// Bottom-right corner.
	gl_Position = position + (projection_ * vec4(font_size_.x, font_size_.y, 0.0f, 0.0f));
	fs_data.texture_position = vec3(1.0f, 1.0f, gs_data[0].texture_layer);
	EmitVertex();
}

void main()
{
	draw_quad(gl_in[0].gl_Position);
	
	EndPrimitive();
}
