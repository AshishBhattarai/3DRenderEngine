#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "light.hpp"

class PointLight : public Light {

private:
	static constexpr float DEFAULT_CONSTANT 	= 	1.0f;
	static constexpr float DEFAULT_LINEAR 		=		0.045f;
	static constexpr float DEFAULT_QUADRATIC 	=		0.0075f;
	static constexpr float DEFAULT_DISTANCE		=		100.0f;

protected:
	// attenuation factors
	float constant;
	float linear;
	float quadratic;
	glm::vec4 attenuation; // constant, linear, quadratic, distance

public:
	PointLight(glm::vec3 position = glm::vec3(0.0f)) :
			Light(position),
			attenuation(DEFAULT_CONSTANT, DEFAULT_LINEAR, DEFAULT_QUADRATIC, DEFAULT_DISTANCE)
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