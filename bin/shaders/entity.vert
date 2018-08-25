#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tex_coords;

out VS_OUT {
	vec2 tex_coords;
	vec3 normal;
	vec3 fragPos; // fragment position
	vec3 toCameraVector; // direction form vertex/fragment to camera
	float fogFactor; // calculate fogFactor in VS
} vs_out;

// fog density & gradient
uniform float fogDensity = 0.0038f;
uniform float fogGradient = 6.0f;

// matrix
uniform mat4 transform_mat;
uniform mat3 normal_mat;

layout (std140) uniform GeneralVSData {
	mat4 projection_mat;
	mat4 view_mat;
	vec3 cameraPosition;
};

void main() {
	vec4 worldPosition = transform_mat * vec4(pos, 1.0f);
	gl_Position = projection_mat * view_mat * worldPosition;

	vs_out.tex_coords = tex_coords;
	vs_out.normal = normal_mat * normal;
	vs_out.fragPos = worldPosition.xyz;
	vs_out.toCameraVector = cameraPosition - worldPosition.xyz;

	float distance = length(vs_out.toCameraVector); //distance for vertex to the camera
	vs_out.fogFactor = exp(-pow((distance*fogDensity), fogGradient));
}