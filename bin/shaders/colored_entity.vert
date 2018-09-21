#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;

out VS_OUT {
	vec3 normal;
	vec3 fragPos;
	vec3 toCameraVector;
	float fogFactor;
} vs_out;

uniform mat4 transform_mat;
uniform mat3 normal_mat;

layout (std140) uniform GeneralVSData {
	mat4 projection_mat;
	mat4 view_mat;
	vec4 cameraPosition;
	float fogDensity;
	float fogGradient;
};

void main() {
	vec4 worldPosition = transform_mat * vec4(pos, 1.0f);
	gl_Position = projection_mat * view_mat * worldPosition;

	vs_out.normal = normal_mat * normal;
	vs_out.fragPos = worldPosition.xyz;
	vs_out.toCameraVector = cameraPosition.xyz - worldPosition.xyz;

	float distance = length(vs_out.toCameraVector);
	vs_out.fogFactor = exp(-pow((distance*fogDensity), fogGradient));
}