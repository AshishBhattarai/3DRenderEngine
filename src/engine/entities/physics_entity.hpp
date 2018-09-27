#ifndef PHYSICS_ENTITY_HPP
#define PHYSICS_ENTITY_HPP

#include "entities/entity.hpp"

#include <memory>

#include "physics/physics_body/physics_body.hpp"

class PhysicsEntity : public Entity {
private:
	std::unique_ptr<PhysicsBody> physicsBody;

public:
	// constructors
	PhysicsEntity(std::shared_ptr<Model> model, std::shared_ptr<CollisionShape> shape,
		const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f), float mass = 0.0f);
	PhysicsEntity(std::shared_ptr<Model> model, std::shared_ptr<CollisionShape>, float mass = 0.0f);

	// methods
	void getTransMatrix(glm::mat4& trans) override;

	void setPosition(const glm::vec3& pos) override;
	void setRotation(const glm::vec3& rot) override;
	void setPitch(float x) override;
	void setYaw(float y) override;
	void setRoll(float z) override;
	void setScale(float s) override;

	glm::vec3 getPosition() const override;
	glm::vec3 getRotation() const override;
	float getPitch() const override;
	float getYaw() const override;
	float getRoll() const override;

	PhysicsBody* getPhysicsBody() const;
};

#endif