#include "point_light.hpp"

PointLight::PointLight(glm::vec3 position) :
		Light(position),
		attenuation(0.0f)
{

}

void PointLight::setAttenuation(float radius, float linear, float minVal) {
	float quadratic = 1.0f/(radius*radius*minVal);
	this->attenuation = glm::vec4(1.0f, linear, quadratic, radius);
}