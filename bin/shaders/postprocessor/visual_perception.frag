#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D frame_texture;

uniform float brightness;
uniform float contrast;
uniform float gamma; // default 1,  gamma(gamma of monitor) 0.x lowers, 1.x highers

void main() {
	FragColor = texture(frame_texture, TexCoords);
	FragColor.rgb += brightness;
	FragColor.rgb = (FragColor.rgb - 0.5f) * (1.0f + contrast) + 0.5f;
	if(gamma != 1.0f)
		FragColor.rgb = pow(FragColor.rgb, vec3(1.0f/gamma));
}