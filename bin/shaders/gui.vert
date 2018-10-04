#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec4 color;

out VS_OUT {
	vec2 texCoords;
	vec4 color;
} vs_out;

uniform mat4 projection_mat;

void main() {
	gl_Position = projection_mat * vec4(position, 0.0f, 1.0f);
	vs_out.texCoords = texCoords;
	vs_out.color = color;
}