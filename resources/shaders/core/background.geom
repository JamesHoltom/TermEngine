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
	vec2 size;
	vec3 color;
} gs_data[];

out FS_DATA
{
	vec2 texture_position;
	vec3 color;
} fs_data;

void draw_quad(vec4 position)
{
	fs_data.color = gs_data[0].color;
	
	// Top-left corner.
	gl_Position = position;
	fs_data.texture_position = vec2(0.0f, 0.0f);
	EmitVertex();
	
	// Bottom-left corner.
	gl_Position = position + (projection_ * vec4(0.0f, gs_data[0].size.y, 0.0f, 0.0f));
	fs_data.texture_position = vec2(0.0f, 1.0f);
	EmitVertex();
	
	// Top-right corner.
	gl_Position = position + (projection_ * vec4(gs_data[0].size.x, 0.0f, 0.0f, 0.0f));
	fs_data.texture_position = vec2(1.0f, 0.0f);
	EmitVertex();
	
	// Bottom-right corner.
	gl_Position = position + (projection_ * vec4(gs_data[0].size.x, gs_data[0].size.y, 0.0f, 0.0f));
	fs_data.texture_position = vec2(1.0f, 1.0f);
	EmitVertex();
}

void main()
{
	draw_quad(gl_in[0].gl_Position);
	
	EndPrimitive();
}
