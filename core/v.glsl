#version 440

layout (location = 0) in vec2 vertex_position;

out vec3 vs_pos;

void main() {
  vs_pos = vec3(vertex_position, 0.0f);
  gl_Position = vec4(vertex_position, 0.0f, 1.0);
}