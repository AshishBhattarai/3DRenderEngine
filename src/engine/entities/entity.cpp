#include "entity.hpp"

#include "utils/math.hpp"

Entity::Entity(std::shared_ptr<Model> model, const glm::vec3& position,
		const glm::vec3& rotation, float scale, int flags) :
	BaseEntity(position, rotation),
	model(model),
	minBB(model->getMinBB()),
	maxBB(model->getMaxBB()),
	occu_query(nullptr),
	scale(scale),
	aabb_mesh(),
	flags(flags)
{}

Entity::Entity(std::shared_ptr<Model> model, const glm::vec3& position,
		const glm::vec3& rotation, float scale) :
	Entity(model, position, rotation, scale, 0)
{
	updateAABB();
}

void Entity::getTransMatrix(glm::mat4& mat) {
	mat = glm::translate(glm::mat4(1.0f), position);
	// roate on y first to match bullet physics rotation
	mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	mat = glm::scale(mat, glm::vec3(scale, scale, scale));
}

void Entity::updateAABB() {
	Math::transformAABB(position, rotation, glm::vec3(scale, scale, scale), minBB, maxBB);
	aabb_mesh.updateData(minBB, maxBB);
}

void Entity::makeOccludee(bool make) {
	// faster than GL_ANY_SAMPLES_PASSED but not always accurate
	if(make) {
		occu_query = std::make_unique<OpenglQuery>(GL_ANY_SAMPLES_PASSED_CONSERVATIVE);
		flags |= EntityFlags::OCCLUDEE;
	} else {
		occu_query.reset();
		flags &= ~EntityFlags::OCCLUDEE;
	}
}