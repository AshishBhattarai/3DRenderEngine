#version 330 core

#define DISCARD_APLAH 0.2
#define MAX_TEXTURE_MAPS 3

#define MAX_POINT_LIGHTS 100
#define MAX_SPOT_LIGHTS 100

out vec4 FragColor;

in VS_OUT {
	vec2 tex_coords;
	vec3 normal;
	vec3 fragPos; // fragment position
	vec3 toCameraVector; // direction form vertex/fragment to camera
	float fogFactor;
} fs_in;

// stores sampled texture maps
vec3 texture_maps[MAX_TEXTURE_MAPS]; // 0 - diffuse, 1 - specular

// color
struct Color {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// lights
// direction light
struct DirLight {
	vec3 direction;

	Color color;
};

// point light
struct PointLight {
	vec3 position;

	// attenuation factors
	float constant;
	float linear;
	float quadratic;

	Color color;
};

// spot light
struct SpotLight {
	vec3 direction;
	PointLight attribs;

	float cutoff;
	float outerCutoff;
};

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;

	bool hasSpecular;
	float specularFactor;
	float shininess;
};

// uniforms
// materials
uniform Material material;

// uniform block
layout(std140) uniform GeneralFSData {
	vec3 fogColor;
	DirLight sun;
};

// calcuate light all - if no specular maps[0] = maps[1] = diffuse-map
vec3 applyLight(Color color, vec3 toLight, vec3 normal, vec3 toCamera) {
	// diffuse
	float diffuse = max(dot(normal, toLight), 0.0f);
	// specular
	vec3 reflectDir = reflect(-toLight, normal);
	float specular = pow(max(dot(reflectDir, toCamera), 0.0f), material.shininess);

	// combine result
	vec3 ambientColor = color.ambient * texture_maps[0];
	vec3 diffuseColor = color.diffuse * diffuse *  texture_maps[0];
	vec3 specularColor = material.specularFactor * color.specular * specular *  texture_maps[1];
	return (ambientColor + diffuseColor + specularColor);
}

// calculate directional light
vec3 applyDirLight(DirLight light, vec3 normal, vec3 toCamera) {
	// frag to light direction
	vec3 toLight = normalize(light.direction);
	// calculate light
	vec3 dirLightColor = applyLight(light.color, toLight, normal, toCamera);

	return dirLightColor;
}

// calculate point light
vec3 applyPointLight(PointLight light, vec3 normal, vec3 toCamera) {
	// frag to light direction
	vec3 toLight = normalize(light.position - fs_in.fragPos);
	// calculate light
	vec3 pointLightColor = applyLight(light.color, toLight, normal, toCamera);
	// attenuation
	float dist = length(light.position - fs_in.fragPos); // distance
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return pointLightColor * attenuation;
}

// calculate spotlight
vec3 applySpotLight(SpotLight light, vec3 normal, vec3 toCamera) {
	// frag to light direction
	vec3 toLight = normalize(light.attribs.position - fs_in.fragPos);
	// calculate light based on point light for attenuation
	vec3 spotLightColor = applyPointLight(light.attribs, normal, toCamera);

	// spotlight calc
	float theta = dot(toLight, normalize(light.direction));
	// spotlight intensity smooth step from outerCutoff(0) to innerCuttoff(1)
	float intensity = smoothstep(light.outerCutoff, light.cutoff, theta);

	return spotLightColor * intensity;
}

void main() {
	// discard if transparent
	vec4 diffuse = texture(material.texture_diffuse, fs_in.tex_coords);
	if(diffuse.a < DISCARD_APLAH)
		discard;

	vec3 outputColor = vec3(0.0f); // output color
	vec3 normal = normalize(fs_in.normal);
	vec3 toCamera = normalize(fs_in.toCameraVector);

	// load diffuse map
	texture_maps[0] = diffuse.rgb;
	// load specular map if it exists
	texture_maps[1] = (material.hasSpecular)?
		texture(material.texture_specular, fs_in.tex_coords).rgb : diffuse.rgb;

	// apply directional light
	outputColor = applyDirLight(sun, normal, toCamera);

	// point, stop -- lights

	// emission

	FragColor = vec4(mix(fogColor, outputColor, fs_in.fogFactor), 1.0f);
}