#ifndef PHYSICS_BODY_HPP
#define PHYSICS_BODY_HPP

#include <memory>
#include <glm/glm.hpp>
#include <bullet/BulletCollision/CollisionDispatch/btCollisionObject.h>

#include "collision_shape.hpp"
#include "physics.hpp"

class PhysicsBody {
protected:
	std::shared_ptr<CollisionShape> collisionShape;
	btCollisionObject* physicsBody;

	PhysicsBody(std::shared_ptr<CollisionShape> collisionShape);

public:
	PhysicsBody(const PhysicsBody&) = delete;
	PhysicsBody& operator=(const PhysicsBody&) = delete;

	virtual ~PhysicsBody();

	// methods
	void getOGLMatrix(void* matData);
	void disableDebugDraw();
	virtual void addToWorld(Physics& phys) = 0;

	// setters
	void setPosition(const glm::vec3& pos);
	void setRotation(const glm::vec3& rot);

	// getters
	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
};

#endif