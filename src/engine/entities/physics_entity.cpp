#include "physics_entity.hpp"

#include "physics/physics_body/static_body.hpp"
#include "physics/physics_body/rigid_body.hpp"

#include "utils/math.hpp"

PhysicsEntity::PhysicsEntity(std::shared_ptr<Model> model, std::shared_ptr<CollisionShape> shape,
		const glm::vec3& position, const glm::vec3& rotation, float mass) :
	Entity(model, glm::vec3(0.0f), glm::vec3(0.0f), 1.0f, EntityFlags::PHYSICS)
{
	if(mass == 0.0f) {
		physicsBody = std::make_unique<StaticBody>(shape, position, rotation);
		flags |= EntityFlags::STATIC;
	} else {
		physicsBody = std::make_unique<RigidBody>(shape, position, rotation, mass);
		flags &= ~EntityFlags::STATIC;
	}
	physicsBody->addToWorld();
	updateAABB();
}

PhysicsEntity::PhysicsEntity(std::shared_ptr<Model> model, std::shared_ptr<CollisionShape> shape, float mass) :
	PhysicsEntity(model, shape, glm::vec3(0.0f), glm::vec3(0.0f), mass)
{}

void PhysicsEntity::getTransMatrix(glm::mat4& trans) {
	physicsBody->getOGLMatrix(glm::value_ptr(trans));
	trans = glm::scale(trans, glm::vec3(scale));
}

// setters
void PhysicsEntity::setPosition(const glm::vec3& pos) {
	physicsBody->setPosition(pos);
}

void PhysicsEntity::setRotation(const glm::vec3& rot) {
	physicsBody->setRotation(rot);
}

void PhysicsEntity::setPitch(float x) {
	glm::vec3 rot = physicsBody->getRotation();
	rot.x = x;
	physicsBody->setRotation(rot);
}

void PhysicsEntity::setYaw(float y) {
	glm::vec3 rot = physicsBody->getRotation();
	rot.y = y;
	physicsBody->setRotation(rot);
}

void PhysicsEntity::setRoll(float z) {
	glm::vec3 rot = physicsBody->getRotation();
	rot.z = z;
	physicsBody->setRotation(rot);
}

void PhysicsEntity::setScale(float s) {
	physicsBody->scaleCollisionShape(glm::vec3(s));
	scale = s;
}

// getters
glm::vec3 PhysicsEntity::getPosition() const {
	return physicsBody->getPosition();
}

glm::vec3 PhysicsEntity::getRotation() const {
	return physicsBody->getRotation();
}

float PhysicsEntity::getPitch() const {
	return physicsBody->getRotation().x;
}

float PhysicsEntity::getYaw() const {
	return physicsBody->getRotation().y;
}

float PhysicsEntity::getRoll() const {
	return physicsBody->getRotation().z;
}

glm::vec3 PhysicsEntity::getMinBB() const {
	return physicsBody->getMinBB();
}

glm::vec3 PhysicsEntity::getMaxBB() const {
	return physicsBody->getMaxBB();
}

void PhysicsEntity::updateAABB() {
	aabb_mesh.updateData(physicsBody->getMinBB(), physicsBody->getMaxBB());
}

PhysicsBody* PhysicsEntity::getPhysicsBody() const {
	return physicsBody.get();
}