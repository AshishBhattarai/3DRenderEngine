#include "entity.hpp"

Entity::Entity(std::shared_ptr<Model> model, const glm::vec3& position,
		const glm::vec3& rotation, float scale) :
		BaseEntity(position, rotation),
		model(model),
		scale(scale)
{}

void Entity::getTransMatrix(glm::mat4& mat) {
	mat = glm::translate(glm::mat4(1.0f), position);
	mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	mat = glm::scale(mat, glm::vec3(scale, scale, scale));
}