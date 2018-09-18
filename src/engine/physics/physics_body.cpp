#include "physics_body.hpp"

PhysicsBody::PhysicsBody(std::shared_ptr<CollisionShape> collisionShape):
	collisionShape(collisionShape)
 {}

PhysicsBody::~PhysicsBody() {
	Physics::getInstance().removePhyiscBody(physicsBody);
	delete physicsBody;
}

void PhysicsBody::getOGLMatrix(void* matData) {
	btTransform trans;
	trans = physicsBody->getWorldTransform();
	trans.getOpenGLMatrix((btScalar*)matData);
}

void PhysicsBody::disableDebugDraw() {
physicsBody->setCollisionFlags(physicsBody->getCollisionFlags() |
		btCollisionObject::CF_DISABLE_VISUALIZE_OBJECT);
}

void PhysicsBody::setPosition(const glm::vec3& pos) {
	btTransform trans = physicsBody->getWorldTransform();
	trans.setOrigin(btVector3(pos.x, pos.y, pos.z));
	physicsBody->setWorldTransform(trans);
}

void PhysicsBody::setRotation(const glm::vec3& rot) {
	btTransform trans = physicsBody->getWorldTransform();

	btQuaternion quat;
	quat.setEuler(glm::radians(rot.y), glm::radians(rot.x), glm::radians(rot.z));
	trans.setRotation(quat);
	physicsBody->setWorldTransform(trans);
}

glm::vec3 PhysicsBody::getPosition() const{
	btVector3 pos = physicsBody->getWorldTransform().getOrigin();
	return glm::vec3(pos.x(), pos.y(), pos.z());
}

glm::vec3 PhysicsBody::getRotation() const {
	btTransform& trans = physicsBody->getWorldTransform();
	glm::vec3 rot;
	trans.getRotation().getEulerZYX(rot.y, rot.x, rot.z);
	rot = glm::degrees(rot);
	return rot;
}