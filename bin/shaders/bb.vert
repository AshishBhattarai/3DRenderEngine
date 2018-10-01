#version 330 core
layout (location = 0) in vec3 position;

layout(std140) uniform GeneralVSData {
	mat4 projection_mat;
	mat4 view_mat;
};

void main() {
	gl_Position = projection_mat * view_mat * vec4(position, 1.0f);
}