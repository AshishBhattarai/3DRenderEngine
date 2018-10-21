#include "lamp.hpp"

Lamp::Lamp(std::shared_ptr<Model> model, glm::vec3 lamp_pos, glm::vec3 light_pos) :
	Entity(model, lamp_pos, glm::vec3(0.0f), 1.0f, 0, Entity::LAMP),
	PointLight(light_pos)
{
	updateAABB();
}