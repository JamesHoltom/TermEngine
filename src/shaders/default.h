namespace term_engine::shaders {
  constexpr char DEFAULT_VERT_GLSL[] = 
  "#version 440 core\n"
  "layout (location = 0) in vec2 vertex_position;\n"
  "layout (location = 1) in vec2 texture_position;\n"
  "layout (location = 2) in vec4 colour;\n"
  "uniform mat4 projection;\n"
  "out FS_DATA\n"
  "{\n"
  "	vec2 texture_position;\n"
  "	vec4 colour;\n"
  "} fs_data;\n"
  "void main()\n"
  "{\n"
  "	gl_Position = projection * vec4(vertex_position, 0.0f, 1.0f);\n"
  "	fs_data.texture_position = texture_position;\n"
  "	fs_data.colour = colour;\n"
  "}";

  constexpr char TEXT_FRAG_GLSL[] = 
  "#version 440 core\n"
  "out vec4 fragment_colour;\n"
  "uniform int is_text;\n"
  "uniform sampler2D fragment_texture;\n"
  "in FS_DATA\n"
  "{\n"
  "	vec2 texture_position;\n"
  "	vec4 colour;\n"
  "} fs_data;\n"
  "void main()\n"
  "{\n"
  " fragment_colour = fs_data.colour;\n"
  " if (is_text > 0.0) {\n"
  "  fragment_colour.w *= texture(fragment_texture, fs_data.texture_position).r;\n"
  " }\n"
  "}";

  constexpr char BACKGROUND_FRAG_GLSL[] = 
  "#version 440 core\n"
  "out vec4 fragment_colour;\n"
  "uniform sampler2D fragment_texture;\n"
  "in FS_DATA\n"
  "{\n"
  "	vec2 texture_position;\n"
  "	vec4 colour;\n"
  "} fs_data;\n"
  "void main()\n"
  "{\n"
  " fragment_colour = texture(fragment_texture, fs_data.texture_position) * fs_data.colour;\n"
  "}";
}
