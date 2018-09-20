#version 330 core

#define TILING_FACTOR 80
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

vec4 texture_maps[MAX_TEXTURE_MAPS]; // 0 - diffuse, 1 - specular

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
	sampler2D texture_red_diffuse;
	sampler2D texture_green_diffuse;
	sampler2D texture_blue_diffuse;
	sampler2D texture_black_diffuse;

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
vec3 calculateLight(Color lightColor, vec3 toLight, vec3 normal, vec3 toCamera) {
	// diffuse
	float diffuse = max(dot(normal, toLight), 0.0f);
	// specular
	vec3 reflectDir = reflect(-toLight, normal);
	float specular = pow(max(dot(reflectDir, toCamera), 0.0f), material.shininess);

	// combine result
	vec3 ambientColor = lightColor.ambient * texture_maps[0].rgb;
	vec3 diffuseColor = lightColor.diffuse * diffuse * texture_maps[0].rgb;
	vec3 specularColor = lightColor.specular * specular * texture_maps[1].rgb;
	return (ambientColor + diffuseColor + specularColor);
}

// calculate directional light
vec3 applyDirLight(DirLight light, vec3 normal, vec3 toCamera) {
	// frag pos to light direction
	vec3 toLight = normalize(light.direction);
	// calculate light
	vec3 dirLightColor = calculateLight(light.color, toLight, normal, toCamera);

	return dirLightColor;
}

// calculate point light
vec3 applyPointLight(PointLight light, vec3 normal, vec3 toCamera) {
	// frag pos to light direction
	vec3 toLight = normalize(light.position - fs_in.fragPos);
	// calculate light
	vec3 pointLightColor = calculateLight(light.color, toLight, normal, toCamera);

	// attenuation
	float dist = length(light.position - fs_in.fragPos); // distance
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return pointLightColor * attenuation;
}

// calculate spotlight
vec3 applySpotLight(SpotLight light, vec3 normal, vec3 toCamera) {
	// frag pos to light direction
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
	vec3 outputColor = vec3(0.0f); // output color
	vec3 normal = normalize(fs_in.normal);
	vec3 toCamera = normalize(fs_in.toCameraVector);

	// sample textures
	vec4 blendMapColor = texture(material.texture_blend, fs_in.tex_coords);
	// black(0, 0, 0) ie r+b+g = 0,
	float blackAmt = 1 - (blendMapColor. r + blendMapColor.g + blendMapColor.b);

	// tile the textures
	vec2 tiledCoords = fs_in.tex_coords * TILING_FACTOR;

	// diffuse colors
	vec4 blackColor = texture(material.texture_black_diffuse, tiledCoords) * blackAmt;
	vec4 redColor	 	=	texture(material.texture_red_diffuse, tiledCoords) * blendMapColor.r;
	vec4 greenColor	=	texture(material.texture_green_diffuse, tiledCoords) * blendMapColor.g;
	vec4 blueColor	=	texture(material.texture_blue_diffuse, tiledCoords) * blendMapColor.b;
	texture_maps[0] = blackColor + redColor + greenColor + blueColor;

	// sepcular colors
	texture_maps[1] = vec4(0.0f);

	// apply directional light
	outputColor = applyDirLight(sun, normal, toCamera);

	// calculate fog
	FragColor = vec4(mix(fogColor, outputColor, fs_in.fogFactor), 1.0f);
}