#include "entity.hpp"

#include "utils/math.hpp"

Entity::Entity(std::shared_ptr<Model> model, const glm::vec3& position,
		const glm::vec3& rotation, float scale) :
		BaseEntity(position, rotation),
		model(model),
		minBB(model->getMinBB()),
		maxBB(model->getMaxBB()),
		scale(scale)
{
	updateAABB();
}

void Entity::getTransMatrix(glm::mat4& mat) {
	mat = glm::translate(glm::mat4(1.0f), position);
	mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	mat = glm::scale(mat, glm::vec3(scale, scale, scale));
}

void Entity::updateAABB() {
	// scale
	glm::mat4 mat(1.0f);
	mat = glm::scale(mat, glm::vec3(scale, scale, scale));
	minBB = glm::vec3(mat * glm::vec4(minBB, 1.0f));
	maxBB = glm::vec3(mat * glm::vec4(maxBB, 1.0f));

	// rotate
	Math::rotateBB(minBB, maxBB, rotation);

	// translate
	mat = glm::translate(glm::mat4(1.0f), position);
	minBB = glm::vec3(mat * glm::vec4(minBB, 1.0f));
	maxBB = glm::vec3(mat * glm::vec4(maxBB, 1.0f));
}