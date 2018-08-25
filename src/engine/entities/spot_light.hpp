#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include "point_light.hpp"

class SpotLight : public PointLight {
private:
	glm::vec3 direction;
	float cutoffAngle; // indder cuttoff angle
	float outer_cutoffAngle;

public:
	SpotLight(glm::vec3 position = glm::vec3(0.0f), glm::vec3 direction = glm::vec3(0.0f),
			glm::vec3 ambient = glm::vec3(0.0f), glm::vec3 diffuse = glm::vec3(0.0f),
			glm::vec3 specular = glm::vec3(0.0f), float cutoffAngle = 0.0f,
			float outer_cutoffAngle = 0.0f, float constant = 0.0f, float linear = 0.0f,
			float quadratic = 0.0f) :
			PointLight(position, ambient, diffuse, specular, constant, linear, quadratic),
			direction(direction),
			cutoffAngle(cutoffAngle),
			outer_cutoffAngle(outer_cutoffAngle)
	{

	}

	// getters
	glm::vec3 getDirection() const {
		return direction;
	}

	float getCutoffAngle() const {
		return cutoffAngle;
	}

	float getOuterCutoffAngle() const {
		return outer_cutoffAngle;
	}

	// setters
	void setDirection(const glm::vec3& dir) {
		direction = dir;
	}

	void setCutoffAngle(float angle) {
		cutoffAngle = angle;
	}

	void setOuterCutoffAngle(float angle) {
		outer_cutoffAngle = angle;
	}
};

#endif