#version 330 core

#define DISCARD_APLAH 0.2
#define MAX_TEXTURE_MAPS 3

#define MAX_POINT_LIGHTS 10
#define MAX_SPOT_LIGHTS 10

out vec4 FragColor;

in VS_OUT {
	vec2 tex_coords;
	vec3 normal;
	vec3 fragPos; // fragment position
	vec3 toCameraVector; // direction form vertex/fragment to camera
	float fogFactor;
} fs_in;

// stores sampled texture maps
vec4 texture_maps[MAX_TEXTURE_MAPS]; // 0 - diffuse, 1 - specular

// color
struct Color {
	vec4 diffuse;
	vec4 specular;
};

// lights
// direction light
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

// spot light
struct SpotLight {
	vec4 direction;
	PointLight attribs;

	float innerCutoff;
	float outerCutoff;
};

struct Material {
	sampler2D texture_diffuse;
	sampler2D texture_specular;

	float shininess;
};

// uniforms
// materials
uniform Material material;

// uniform block
layout(std140) uniform GeneralFSData {
	vec4 fogColor;
	float ambientFactor;
	DirLight sun;
	PointLight pointLights[MAX_POINT_LIGHTS];
};

// calcuate lights
vec3 calculateLight(Color lightColor, vec3 toLight, vec3 normal, vec3 toCamera) {
	// diffuse
	float diffuse = max(dot(normal, toLight), 0.0f);
	// specular
	vec3 reflectDir = reflect(-toLight, normal);
	float specular = pow(max(dot(reflectDir, toCamera), 0.0f), material.shininess);

	// combine result
	vec3 diffuseColor = lightColor.diffuse.xyz * diffuse *  texture_maps[0].rgb;
	vec3 specularColor = lightColor.specular.xyz * specular *  texture_maps[1].rgb;
	return (diffuseColor + specularColor);
}

// calculate directional light
vec3 applyDirLight(DirLight light, vec3 normal, vec3 toCamera) {
	// frag to light direction
	vec3 toLight = normalize(light.direction.xyz);
	// calculate light
	vec3 dirLightColor = calculateLight(light.color, toLight, normal, toCamera);

	return dirLightColor;
}

// calculate point light
vec3 applyPointLight(PointLight light, vec3 normal, vec3 toCamera) {
	// frag to light direction
	vec3 toLight = normalize(light.position.xyz - fs_in.fragPos);
	// calculate light
	vec3 pointLightColor = calculateLight(light.color, toLight, normal, toCamera);
	// attenuation
	float dist = length(light.position.xyz - fs_in.fragPos); // distance
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));

	return pointLightColor * attenuation;
}

// calculate spotlight
// vec3 applySpotLight(SpotLight light, vec3 normal, vec3 toCamera) {
// 	// frag to light direction
// 	vec3 toLight = normalize(light.attribs.position.xyz - fs_in.fragPos);
// 	// calculate light based on point light for attenuation
// 	vec3 spotLightColor = applyPointLight(light.attribs, normal, toCamera);

// 	// spotlight calc
// 	float theta = dot(toLight, normalize(light.direction.xyz));
// 	// spotlight intensity smooth step from outerCutoff(0) to innerCuttoff(1)
// 	float intensity = smoothstep(light.outerCutoff, light.innerCutoff, theta);

// 	return spotLightColor * intensity;
// }

void main() {
	// load diffuse
	texture_maps[0] = texture(material.texture_diffuse, fs_in.tex_coords);
	// discard if transparent
	if(texture_maps[0].a < DISCARD_APLAH)
		discard;

	vec3 outputColor = vec3(0.0f); // output color
	vec3 normal = normalize(fs_in.normal);
	vec3 toCamera = normalize(fs_in.toCameraVector);

	// load specular map - use diffuse if not available
	texture_maps[1] = (textureSize(material.texture_specular, 0).x == 1.0f)?
										texture_maps[0] : texture(material.texture_specular, fs_in.tex_coords);

	// apply directional light
	outputColor = applyDirLight(sun, normal, toCamera);

	// point lights
	for(int i = 0; i < 4; ++i) {
		if(pointLights[i].attenuation.x != 0.0f)
			outputColor += applyPointLight(pointLights[i], normal, toCamera);
	}

	// ambinet
	outputColor = max(outputColor, ambientFactor * texture_maps[0].xyz);

	// emission

	FragColor = vec4(mix(fogColor.xyz, outputColor, fs_in.fogFactor), 1.0f);
}