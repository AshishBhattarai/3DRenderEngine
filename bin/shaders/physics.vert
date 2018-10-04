#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
out vec3 out_color;

uniform bool isContactPoint;

layout (std140) uniform GeneralVSData {
	mat4 projection_mat;
	mat4 view_mat;
	vec3 cameraPosition;
};

void main() {
	vec4 position = projection_mat * view_mat * vec4(pos, 1.0f);
	gl_PointSize = (isContactPoint)? 4.0f : 1.0f;

	// position.z = 0;
	gl_Position = position;
	out_color = color;
}