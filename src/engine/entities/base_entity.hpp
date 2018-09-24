#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include <glm/glm.hpp>

class BaseEntity {
protected:
	glm::vec3 position;
	glm::vec3 rotation;

public:
	BaseEntity(const glm::vec3& position = glm::vec3(0.0f),
		const glm::vec3& rotation = glm::vec3(0.0f)) :
		position(position),
		rotation(rotation)
		{}

	virtual void setPosition(const glm::vec3& pos) {
		position = pos;
	}

	virtual void setRotation(const glm::vec3& rot) {
		rotation = rot;
	}

	virtual void setPitch(float x) {
		rotation.x = x;
	}

	virtual void setYaw(float y) {
		rotation.y = y;
	}

	virtual void setRoll(float z) {
		rotation.z = z;
	}

	virtual glm::vec3 getPosition() const {
		return position;
	}

	virtual glm::vec3 getRotation() const {
		return rotation;
	}

	virtual float getPitch() const {
		return rotation.x;
	}

	virtual float getYaw() const {
		return rotation.y;
	}

	virtual float getRoll() const {
		return rotation.z;
	}
};

#endif