#include "rigid_body.hpp"

#include "../physics_world.hpp"

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
	this->physicsBody = new btRigidBody(mass, motionState, cShape, inertia);
}

void RigidBody::addToWorld() {
	PhysicsWorld::getInstance().addRigidBody((btRigidBody*)physicsBody);
}

RigidBody::~RigidBody() {
	delete motionState;
}