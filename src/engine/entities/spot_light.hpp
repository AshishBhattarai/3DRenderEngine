#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include "point_light.hpp"

class SpotLight : public PointLight {
private:
	static constexpr float DEFAULT_INNER_CUTOFF = 12.0f;
	static constexpr float DEFAULT_OUTER_CUTOFF = 18.0f;

	glm::vec3 direction;
	glm::vec2 cutoffAngle; // inner, outer cutoff

public:
	SpotLight(glm::vec3 position = glm::vec3(0.0f)) :
			PointLight(position),
			direction(direction),
			cutoffAngle(DEFAULT_INNER_CUTOFF, DEFAULT_OUTER_CUTOFF)
	{

	}

	// getters
	glm::vec3 getDirection() const {
		return direction;
	}

	glm::vec2 getCutoffAngle() const {
		return cutoffAngle;
	}

	// setters
	void setCutoffAngle(const glm::vec2& cutoffAngle) {
		this->cutoffAngle = cutoffAngle;
	}
};

#endif