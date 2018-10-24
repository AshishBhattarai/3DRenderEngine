#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec2 texCoords;
	vec4 color;
} fs_in;

uniform sampler2D tex;

void main() {
	FragColor = fs_in.color * texture(tex, fs_in.texCoords);
	FragColor.rgb = pow(FragColor.rgb, vec3(2.2f));
}