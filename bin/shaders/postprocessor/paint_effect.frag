#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D tex;
uniform vec2 scr_size;
uniform int radius;

void main() {
	float n = float((radius + 1) * (radius + 1));

	vec3 m[4];
	vec3 s[4];
	for(int k = 0; k < 4; ++k) {
		m[k] = vec3(0.0f);
		s[k] = vec3(0.0f);
	}

	// 3rd quadrant
	for(int j = -radius; j <= 0; ++j) {
		for(int i = -radius; i <= 0; ++i) {
			vec3 c = texture(tex, TexCoords + vec2(i, j) / scr_size).rgb;
			m[0] += c;
			s[0] += c * c;
		}
	}

	// 4th quadrant
	for(int j = -radius; j <= 0; ++j) {
		for(int i = 0; i <= radius; ++i) {
			vec3 c = texture(tex, TexCoords + vec2(i, j) / scr_size).rgb;
			m[1] += c;
			s[1] += c * c;
		}
	}

	// 1st quadrant
	for(int j = 0; j <= radius; ++j) {
		for(int i = 0; i <= radius; ++i) {
			vec3 c = texture(tex, TexCoords + vec2(i, j) / scr_size).rgb;
			m[2] += c;
			s[2] += c * c;
		}
	}

	// 2nd quadrant
	for(int j = 0; j <= radius; ++j) {
		for(int i = -radius; i <= 0; ++i) {
			vec3 c = texture(tex, TexCoords + vec2(i, j) / scr_size).rgb;
			m[3] += c;
			s[3] += c * c;
		}
	}

	float min_sigma2 = 1e+2; // 1 * 10^2
	for(int k = 0; k < 4; ++k) {
		m[k] /= n;
		s[k] = abs(s[k] / n - m[k] * m[k]);

		float sigma2 = s[k].r + s[k].g + s[k].b;
		if(sigma2 < min_sigma2) {
			min_sigma2 = sigma2;
			FragColor = vec4(m[k], 1.0f);
		}
	}
}