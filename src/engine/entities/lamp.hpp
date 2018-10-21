#ifndef LAMP_HPP
#define LAMP_HPP

#include "point_light.hpp"
#include "entity.hpp"

class Lamp : public Entity, public PointLight {
public:
	Lamp(std::shared_ptr<Model> model, glm::vec3 lamp_pos = glm::vec3(0.0f), glm::vec3 light_pos = glm::vec3(0.0f));

	// use entity position methods
	using Entity::setPosition;
	using Entity::getPosition;

	// wrap point lights' position methods
	void setLightPosition(const glm::vec3& position) {
		PointLight::setPosition(position);
	}

	glm::vec3 getLightPosition() {
		return PointLight::getPosition();
	}
};


#endif