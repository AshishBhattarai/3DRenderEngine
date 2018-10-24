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
	PointLight(glm::vec3 position = glm::vec3(0.0f));

	// getters
	glm::vec4 getAttenuation() const {
		return attenuation;
	}

	// setters
	void setAttenuation(const glm::vec4& attenuation) {
		this->attenuation = attenuation;
	}

	/**
	 * @parmas :
	 * 	1 - radius of the light
	 * 	2 - linear factor
	 * 	3 - minimum attenuation value after which the light is cut off
	 * 			(ie attenuation = 0.0f if its less than 0.01f)
	 */
	void setAttenuation(float radius, float linear = 0.0f, float minVal = 0.01f);
};


#endif