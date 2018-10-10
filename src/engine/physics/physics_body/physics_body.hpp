#ifndef PHYSICS_BODY_HPP
#define PHYSICS_BODY_HPP

#include <memory>
#include <glm/glm.hpp>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

#include "../collision_shape/collision_shape.hpp"

class PhysicsBody {
public:
	enum Type {
		STATIC = 0,
		RIGID  = 1
	};

protected:
	std::shared_ptr<CollisionShape> collisionShape;
	btCollisionObject* physicsBody;
	Type type;

	PhysicsBody(std::shared_ptr<CollisionShape> collisionShape, Type type);

public:
	PhysicsBody(const PhysicsBody&) = delete;
	PhysicsBody& operator=(const PhysicsBody&) = delete;

	virtual ~PhysicsBody();

	// methods
	void getOGLMatrix(void* matData);
	void disableDebugDraw();
	void updateAABB();
	void scaleCollisionShape(const glm::vec3& scale);
	virtual void addToWorld();
	void removeFromWorld();

	// setters
	void setPosition(const glm::vec3& pos);
	void setRotation(const glm::vec3& rot);

	void setFriction(float fri);
	void setRestitution(float res);
	void setRollingFriction(float fri);

	// getters
	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
	glm::vec3 getMinBB() const;
	glm::vec3 getMaxBB() const;
	PhysicsBody::Type getType() const;
};

#endif