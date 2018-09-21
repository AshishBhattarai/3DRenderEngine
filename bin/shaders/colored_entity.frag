#version 330 core

#define MAX_POINT_LIGHTS 100
#define MAX_SPOT_LIGHTS 100

out vec4 FragColor;

in VS_OUT {
	vec3 normal;
	vec3 fragPos;
	vec3 toCameraVector;
	float fogFactor;
} fs_in;

struct Color {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material {
	Color color;
	float shininess;
};

struct DirLight {
	vec3 direction;
	Color color;
};

struct PointLight {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	Color color;
};

struct SpotLight {
	vec3 direction;
	float innerCutoff;
	float outerCutoff;
	PointLight attribs;
};

layout(std140) uniform GeneralFSData {
	vec3 fogColor;
	DirLight sun;
};

uniform Material material;

vec3 calcuateLight(Color lightColor, vec3 toLight, vec3 toCamera, vec3 normal) {
	float diffuse = max(dot(normal, toLight), 0.0f);

	vec3 reflectDir = reflect(-toLight, normal);
	float specular = pow(max(dot(reflectDir, toCamera), 0.0f), material.shininess);

	vec3 ambientColor = lightColor.ambient * material.color.ambient * material.color.diffuse;
	vec3 diffuseColor = lightColor.diffuse * diffuse * material.color.diffuse;
	vec3 specularColor = lightColor.specular * specular * material.color.specular;

	return ambientColor + diffuseColor + specularColor;
}

vec3 applyDirLight(DirLight light, vec3 normal, vec3 toCamera) {
	vec3 toLight = normalize(light.direction);
	return calcuateLight(light.color, toLight, toCamera, normal);
}

vec3 applyPointLight(PointLight light, vec3 normal, vec3 toCamera) {
	vec3 toLight = normalize(light.position - fs_in.fragPos);
	vec3 pointLightColor = calcuateLight(light.color, toLight, toCamera, normal);

	float dist = length(light.position - fs_in.fragPos); // distance
	float attenuation = 1.0f/(light.constant + light.linear * dist + light.quadratic + (dist * dist));

	return pointLightColor * attenuation;
}

vec3 applySpotLight(SpotLight light, vec3 normal, vec3 toCamera) {
	vec3 toLight = normalize(light.attribs.position - fs_in.fragPos);
	vec3 spotLightColor = applyPointLight(light.attribs, normal, toCamera);

	float theta = dot(toLight, normalize(light.direction));
	float intensity = smoothstep(light.outerCutoff, light.innerCutoff, theta);

	return spotLightColor * intensity;
}

void main() {
	vec3 outputColor = vec3(0.0f);
	vec3 normal = normalize(fs_in.normal);
	vec3 toCamera = normalize(fs_in.toCameraVector);

	outputColor = applyDirLight(sun, normal, toCamera);

	FragColor = vec4(mix(fogColor, outputColor, fs_in.fogFactor), 1.0f);
}