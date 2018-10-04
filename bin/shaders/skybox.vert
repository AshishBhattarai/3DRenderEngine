#version 330 core
layout(location = 0) in vec3 position;
out vec3 texDir; // texture direction vector

layout(std140) uniform GeneralVSData {
	mat4 projection_mat;
};

// view * rotation;
uniform mat4 skybox_mat;

void main() {
	// direction for texture is position for vertex
	texDir = normalize(position);

	vec4 pos = projection_mat * skybox_mat * vec4(position, 1.0f);
	gl_Position = pos.xyww; // z always == 1, skybox far
}