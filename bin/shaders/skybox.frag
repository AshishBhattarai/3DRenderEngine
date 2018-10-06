#version 330 core
out vec4 FragColor;
in vec3 texDir;

uniform samplerCube skyboxTexture;

// normalized upper&lower limit for skybox fog
uniform float fog_upper = 0.1f;
uniform float fog_lower = 0.0f;

uniform bool enable_fog;

layout(std140) uniform GeneralFSData {
	vec3 fogColor;
};

void main() {
	vec4 color =  texture(skyboxTexture, texDir);

	if(enable_fog) {
		float factor = smoothstep(fog_lower, fog_upper, texDir.y);
		color = mix(vec4(fogColor, 1.0f), color, factor);
	}
	FragColor = color;
}