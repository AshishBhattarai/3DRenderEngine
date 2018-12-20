#ifndef VISUAL_PERCEPTION_HPP
#define VISUAL_PERCEPTION_HPP

#include "../shader.hpp"

/* postprocessor shader for basic visual preception such as brightness and contrast. */

class VisualPerception : public Shader {
private:
	static constexpr std::string_view VERTEX_FILE = "shaders/postprocessor/postprocessor.vert";
	static constexpr std::string_view FRAGMENT_FILE = "shaders/postprocessor/visual_perception.frag";

	GLint loc_brightness;
	GLint loc_contrast;
	GLint loc_gamma;

public:
	VisualPerception();

	void loadBrightness(float brightness) {
		uniform1f(loc_brightness, brightness);
	}

	void loadContrast(float contrast) {
		uniform1f(loc_contrast, contrast);
	}

	void loadGamma(float gamma) {
		uniform1f(loc_gamma, gamma);
	}

};

#endif

