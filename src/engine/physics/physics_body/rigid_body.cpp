#include "rigid_body.hpp"

#include "../physics_world.hpp"
#include "utils/type_conversion.hpp"

RigidBody::RigidBody(std::shared_ptr<CollisionShape> collisionShape,
	const glm::vec3& position, const glm::vec3& rotation, float mass):
	PhysicsBody(collisionShape, PhysicsBody::RIGID)
{
	// create a rigid body
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(position.x, position.y, position.z));

	btQuaternion quat;
	// .setEuler takes yaw,pitch,roll
	quat.setEuler(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));
	trans.setRotation(quat);

	btCollisionShape* cShape = collisionShape->shape;
	btVector3 inertia(0.0f, 0.0f, 0.0f);
	if(mass)
		cShape->calculateLocalInertia(mass, inertia);

	// motion state
	motionState = new btDefaultMotionState(trans);
	// rigidbody constructor
	this->rigidBody = new btRigidBody(mass, motionState, cShape, inertia);
	this->physicsBody = rigidBody;
}

RigidBody::~RigidBody() {
	delete motionState;
}

void RigidBody::addToWorld() {
	PhysicsWorld::getInstance().addRigidBody(static_cast<btRigidBody*>(physicsBody));
}

glm::vec3 RigidBody::getLinearVelocity() {
	return VEC3::btToGlm(rigidBody->getLinearVelocity());
}

void RigidBody::setFriction(float fri) {
	rigidBody->setFriction(fri);
}

void RigidBody::setRestitution(float res) {
	rigidBody->setRestitution(res);
}

void RigidBody::setRollingFriction(float fri) {
	rigidBody->setRollingFriction(fri);
}

void RigidBody::applyCentralImpulse(const glm::vec3& imp) {
	rigidBody->applyCentralImpulse(VEC3::glmToBt(imp));
}