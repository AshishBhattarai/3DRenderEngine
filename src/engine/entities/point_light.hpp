#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "light.hpp"

class PointLight : public Light {
protected:
	// attenuation factors
	float constant;
	float linear;
	float quadratic;

public:
	PointLight(glm::vec3 position = glm::vec3(0.0f),
			glm::vec3 diffuse  = glm::vec3(0.0f),	glm::vec3 specular = glm::vec3(0.0f),
			float constant = 0.0f, float linear = 0.0f, float quadratic = 0.0f) :
			Light(position, diffuse, specular),
			constant(constant),
			linear(linear),
			quadratic(quadratic)
	{}

	// getters
	float getConstantFactor() const {
		return constant;
	}

	float getLinearFactor() const {
		return linear;
	}

	float getQuadraticFactor() const {
		return quadratic;
	}

	// setters
	void setConstantFactor(float factor) {
		constant = factor;
	}

	void setLinearFactor(float factor) {
		linear = factor;
	}

	void setQuadraticFactor(float factor) {
		quadratic = factor;
	}
};


#endif