#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include <glm/glm.hpp>

class BaseEntity {
protected:
	glm::vec3 position;

	// rotation
	float pitch;
	float yaw;
	float roll;

public:
	BaseEntity(glm::vec3 position = glm::vec3(0.0f), float pitch = 0.0f,
		float yaw = 0.0f, float roll = 0.0f) :
		position(position),
		pitch(pitch),
		yaw(yaw),
		roll(roll) {}

	void setPosition(glm::vec3 pos) {
		position = pos;
	}

	void setPitch(float x) {
		pitch = x;
	}

	void setYaw(float y) {
		yaw = y;
	}

	void setRoll(float z) {
		roll = z;
	}

	glm::vec3 getPosition() const {
		return position;
	}

	float getPitch() const {
		return pitch;
	}

	float getYaw() const {
		return yaw;
	}

	float getRoll() const {
		return roll;
	}
};

#endif