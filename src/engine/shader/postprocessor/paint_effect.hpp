#ifndef PAINT_EFFECT_HPP
#define PAINT_EFFECT_HPP

#include "../shader.hpp"

class PaintEffect : public Shader {
private:
	static constexpr std::string_view VERTEX_FILE = "shaders/postprocessor/postprocessor.vert";
	static constexpr std::string_view FRAGMENT_FILE =  "shaders/postprocessor/paint_effect.frag";

	GLint loc_radius;
	GLint loc_scrSize;

public:
	PaintEffect();

	void loadRadius(int radius) {
		uniform1i(loc_radius, radius);
	}

	void loadScreenSize(const glm::vec2& size) {
		uniform2fv(loc_scrSize, size);
	}
};

#endif