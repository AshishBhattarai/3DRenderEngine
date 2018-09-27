#ifndef RIGID_BODY_HPP
#define RIGID_BODY_HPP

#include <memory>
#include <glm/glm.hpp>
#include <bullet/BulletDynamics/Dynamics/btRigidBody.h>

#include "physics_body.hpp"

class RigidBody : public PhysicsBody {
private:
	btDefaultMotionState* motionState;

public:
	RigidBody(std::shared_ptr<CollisionShape> collisionShape, const glm::vec3& position = glm::vec3(0.0f),
		const glm::vec3& rotation = glm::vec3(0.0f), float mass = 0.0f);

	~RigidBody();

	void addToWorld() override;
};

#endif