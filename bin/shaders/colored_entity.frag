#version 330 core

#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

out vec4 FragColor;

in VS_OUT {
	vec3 normal;
	vec3 fragPos;
	vec3 toCameraVector;
	float fogFactor;
} fs_in;

struct Color {
	vec4 diffuse;
	vec4 specular;
};

struct Material {
	Color color;
	float shininess;
};

struct DirLight {
	vec4 direction;
	Color color;
};

// point light
struct PointLight {
	vec4 position;
	Color color;

	// attenuation factors
	vec4 attenuation; // constant, linear, quadratic
};

struct SpotLight {
	vec4 direction;
	float innerCutoff;
	float outerCutoff;
	PointLight attribs;
};

layout(std140) uniform GeneralFSData {
	vec4 fogColor;
	float ambientFactor;
	DirLight sun;
	int numPointLight;
	PointLight pointLights[MAX_POINT_LIGHTS];
};

uniform Material material;

vec3 calculateLight(Color lightColor, vec3 toLight, vec3 normal, vec3 toCamera) {
	float diffuse = max(dot(normal, toLight), 0.0f);

	vec3 halfway = normalize(toLight + toCamera);
	float specular = pow(max(dot(normal, halfway), 0.0f), material.shininess);

	vec3 diffuseColor = lightColor.diffuse.xyz * diffuse * material.color.diffuse.xyz;
	vec3 specularColor = lightColor.specular.xyz * specular * material.color.specular.xyz;

	return diffuseColor + specularColor;
}

vec3 applyDirLight(DirLight light, vec3 normal, vec3 toCamera) {
	vec3 toLight = normalize(light.direction.xyz);
	return calculateLight(light.color, toLight, toCamera, normal);
}

vec3 applyPointLight(PointLight light, vec3 normal, vec3 toCamera) {
	vec3 toLight = normalize(light.position.xyz - fs_in.fragPos);
	vec3 pointLightColor = calculateLight(light.color, toLight, normal, toCamera);
	float dist = length(light.position.xyz - fs_in.fragPos);

	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));

	return pointLightColor * attenuation;
}

// vec3 applySpotLight(SpotLight light, vec3 normal, vec3 toCamera) {
// 	vec3 toLight = normalize(light.attribs.position.xyz - fs_in.fragPos);
// 	vec3 spotLightColor = applyPointLight(light.attribs, normal, toCamera);

// 	float theta = dot(toLight, normalize(light.direction.xyz));
// 	float intensity = smoothstep(light.outerCutoff, light.innerCutoff, theta);

// 	return spotLightColor * intensity;
// }

void main() {
	vec3 outputColor = vec3(0.0f);
	vec3 normal = normalize(fs_in.normal);
	vec3 toCamera = normalize(fs_in.toCameraVector);

	outputColor = applyDirLight(sun, normal, toCamera);

	// point lights
	for(int i = 0; i < numPointLight; ++i) {
		if(pointLights[i].attenuation.x != 0.0f) {
			outputColor += applyPointLight(pointLights[i], normal, toCamera);
		}
	}

	// ambinet
	outputColor = max(outputColor, ambientFactor * material.color.diffuse.xyz);

	FragColor = vec4(mix(fogColor.xyz, outputColor, fs_in.fogFactor), 1.0f);
}