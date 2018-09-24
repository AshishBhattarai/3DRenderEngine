#include "static_body.hpp"

StaticBody::StaticBody(std::shared_ptr<CollisionShape> collisionShape,
	const glm::vec3& position, const glm::vec3& rotation) :
	PhysicsBody(collisionShape, PhysicsBody::STATIC)
{
	btTransform trans;
	trans.setIdentity();
	// position
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	// rotation
	btQuaternion quat;
	quat.setEuler(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
	trans.setRotation(quat);

	physicsBody = new btCollisionObject();
	physicsBody->setWorldTransform(trans);
	physicsBody->setCollisionShape(collisionShape->shape);
	physicsBody->setCollisionFlags(physicsBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
	physicsBody->setActivationState(ISLAND_SLEEPING); // puts object to sleep - important for static objects
}