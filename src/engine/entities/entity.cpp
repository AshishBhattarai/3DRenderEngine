#include "entity.hpp"

Entity::Entity(std::shared_ptr<Model> model, glm::vec3 position,
		float pitch, float yaw, float roll, float scale) :
		BaseEntity(position, pitch, yaw, roll),
		scale(scale),
		model(model)
{}

