#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "base_entity.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

// A freely moving camera
// TODO: add in 3rd person mode

enum Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera : public BaseEntity {
private:
	// default settings
	static constexpr float YAW 					=	0.0f;
	static constexpr float PITCH				=	0.0f;
	static constexpr float ROLL					= 0.0f;
	static constexpr float SPEED 				= 5.5f;
	static constexpr float SENSITIVITY	= 4.0f;
	static constexpr float ZOOM					= 45.0f;

	// attributes
	glm::quat orientation;
	glm::mat4 viewMat;

	// direction vectors
	glm::vec3 front; //	z
	glm::vec3 right; // x
	glm::vec3 up;		 // y

	float movementSpeed;
	float mouseSensitivity;

	// update camera orientation and direction vectors
	void updateCameraDirection();

	void calcuateViewMatrix() {
		viewMat = glm::translate(glm::mat4_cast(orientation), -position);
	}

public:
	Camera(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& rotation = glm::vec3(0.0f));

	void processKeyboard(Movement dir, float dt);
	void processMouseMovement(float xoffset, float yoffset, float dt);

	// get view matrix
	glm::mat4 getViewMatrix() const {
		// we translate then rotate so that camera doesn't rotate around old position
		return viewMat;
	}

	void setPosition(const glm::vec3& pos) override {
		position = pos;
		updateCameraDirection();
	}

	void setRotation(const glm::vec3& rot) override {
		rotation = rot;
		updateCameraDirection();
	}

	void setPitch(float p) override {
		rotation.x = p;
		updateCameraDirection();
	}

	void setYaw(float y) override {
		rotation.y = y;
		updateCameraDirection();
	}

	void setRoll(float r) override {
		rotation.z = r;
		updateCameraDirection();
	}
};

#endif