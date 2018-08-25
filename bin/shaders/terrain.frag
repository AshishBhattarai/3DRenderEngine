#version 330 core

#define TILING_FACTOR 80

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
	// texture map / splat
	sampler2D texture_blend;
	sampler2D texture_red;
	sampler2D texture_green;
	sampler2D texture_blue;
	sampler2D texture_black;

	float specularFactor;
	float shininess;
};

// uniforms
// materials
uniform Material material;

layout(std140) uniform GeneralFSData {
	vec3 fogColor;
	DirLight sun;
};

// calcuate light
vec3 applyLight(Color color, vec3 toLight, vec3 normal, vec3 toCamera, vec3 totalColor) {
	// diffuse
	float diffuse = max(dot(normal, toLight), 0.0f);
	// specular
	vec3 reflectDir = reflect(-toLight, normal);
	float specular = pow(max(dot(reflectDir, toCamera), 0.0f), material.shininess);

	// combine result
	vec3 ambientColor = color.ambient * totalColor;
	vec3 diffuseColor = color.diffuse * diffuse * totalColor;
	vec3 specularColor = color.specular * material.specularFactor * specular * totalColor;
	return (ambientColor + diffuseColor + specularColor);
}

// calculate directional light
vec3 applyDirLight(DirLight light, vec3 normal, vec3 toCamera, vec3 totalColor) {
	// frag pos to light direction
	vec3 toLight = normalize(light.direction);
	// calculate light
	vec3 dirLightColor = applyLight(light.color, toLight, normal, toCamera, totalColor);

	return dirLightColor;
}

// calculate point light
vec3 applyPointLight(PointLight light, vec3 normal, vec3 toCamera, vec3 totalColor) {
	// frag pos to light direction
	vec3 toLight = normalize(light.position - fs_in.fragPos);
	// calculate light
	vec3 pointLightColor = applyLight(light.color, toLight, normal, toCamera, totalColor);

	// attenuation
	float dist = length(light.position - fs_in.fragPos); // distance
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return pointLightColor * attenuation;
}

// calculate spotlight
vec3 applySpotLight(SpotLight light, vec3 normal, vec3 toCamera, vec3 totalColor) {
	// frag pos to light direction
	vec3 toLight = normalize(light.attribs.position - fs_in.fragPos);
	// calculate light based on point light for attenuation
	vec3 spotLightColor = applyPointLight(light.attribs, normal, toCamera, totalColor);

	// spotlight calc
	float theta = dot(toLight, normalize(light.direction));
	// spotlight intensity smooth step from outerCutoff(0) to innerCuttoff(1)
	float intensity = smoothstep(light.outerCutoff, light.cutoff, theta);

	return spotLightColor * intensity;
}

void main() {
	vec3 outputColor = vec3(0.0f); // output color
	vec3 normal = normalize(fs_in.normal);
	vec3 toCamera = normalize(fs_in.toCameraVector);

	// sample textures
	vec4 blendMapColor = texture(material.texture_blend, fs_in.tex_coords);
	// black(0, 0, 0) ie r+b+g = 0,
	float blackAmt = 1 - (blendMapColor. r + blendMapColor.g + blendMapColor.b);

	// tile the textures
	vec2 tiledCoords = fs_in.tex_coords * TILING_FACTOR;

	vec4 blackColor = texture(material.texture_black, tiledCoords) * blackAmt;
	vec4 redColor	 	=	texture(material.texture_red, tiledCoords) * blendMapColor.r;
	vec4 greenColor	=	texture(material.texture_green, tiledCoords) * blendMapColor.g;
	vec4 blueColor	=	texture(material.texture_blue, tiledCoords) * blendMapColor.b;


	vec4 totalColor = blackColor + redColor + greenColor + blueColor;

	// apply directional light
	outputColor = applyDirLight(sun, normal, toCamera, totalColor.xyz);

	// calculate fog
	FragColor = vec4(mix(fogColor, outputColor, fs_in.fogFactor), 1.0f);
}