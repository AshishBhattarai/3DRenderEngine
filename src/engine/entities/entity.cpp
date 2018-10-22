#include "entity.hpp"

#include "utils/math.hpp"

Entity::Entity(std::shared_ptr<Model> model, const glm::vec3& position,
		const glm::vec3& rotation, float scale, int flags, Type type) :
	BaseEntity(position, rotation),
	model(model),
	occu_query(nullptr),
	scale(scale),
	curr_aabb(model->getMinBB(), model->getMaxBB()),
	last_aabb(curr_aabb),
	aabb_mesh(),
	flags(flags),
	type(type)
{}

Entity::Entity(std::shared_ptr<Model> model, const glm::vec3& position,
		const glm::vec3& rotation, float scale) :
	Entity(model, position, rotation, scale, EntityFlags::STATIC, BASIC)
{
	updateAABB();
}

Entity::~Entity() {}

void Entity::getTransMatrix(glm::mat4& mat) {
	mat = glm::translate(glm::mat4(1.0f), position);
	// roate on y first to match bullet physics rotation
	mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	mat = glm::scale(mat, glm::vec3(scale, scale, scale));
}

void Entity::updateAABB() {
	curr_aabb.transform(position, rotation, glm::vec3(scale));
	if(curr_aabb == last_aabb)
		return;
	aabb_mesh.updateData(curr_aabb);
	last_aabb = curr_aabb;
}

void Entity::makeOccludee(bool make) {
	// faster than GL_ANY_SAMPLES_PASSED but not always accurate
	if(make) {
		occu_query = std::make_unique<OpenglQuery>(GL_ANY_SAMPLES_PASSED); // GL_ANY_SAMPLES_PASSED_CONSERVATIVE opengl 4.3+
		flags |= EntityFlags::OCCLUDEE;
	} else {
		occu_query.reset();
		flags &= ~EntityFlags::OCCLUDEE;
	}
}