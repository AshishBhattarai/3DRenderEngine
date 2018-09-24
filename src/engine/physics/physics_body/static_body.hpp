#ifndef STATIC_BODY_HPP
#define STATIC_BODY_HPP

#include "physics_body.hpp"

class StaticBody : public PhysicsBody {

public:
	StaticBody(std::shared_ptr<CollisionShape> collisionShape,
		const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f));
};

#endif