#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "light.hpp"

class PointLight : public Light {
protected:
	// attenuation factors
	float constant;
	float linear;
	float quadratic;
	glm::vec4 attenuation; // constant, linear, quadratic, distance

public:
	PointLight(glm::vec3 position = glm::vec3(0.0f)) :
			Light(position),
			attenuation(0.0f)
	{}

	// getters
	glm::vec4 getAttenuation() const {
		return attenuation;
	}

	// setters
	void setAttenuation(const glm::vec4& attenuation) {
		this->attenuation = attenuation;
	}
};


#endif