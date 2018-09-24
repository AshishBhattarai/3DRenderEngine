#include "camera.hpp"

// update camera orientation and axis
void Camera::updateCameraDirection() {
	// update orientation
	orientation = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	orientation *= glm::angleAxis(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	// orientation *= glm::quat(glm::angleAxis(glm::radians(roll), glm::vec3(0.0f, 0.0f, 1.0f)));

	orientation = glm::normalize(orientation);

	calcuateViewMatrix();

	// update vectors
	front = glm::vec3(viewMat[0][2], viewMat[1][2], viewMat[2][2]);
	right = glm::vec3(viewMat[0][0], viewMat[1][0], viewMat[2][0]);
	up 		= glm::vec3(viewMat[0][1], viewMat[1][1], viewMat[2][1]);
}

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation):
		BaseEntity(position, rotation),
		movementSpeed(SPEED),
		mouseSensitivity(SENSITIVITY)
{
	updateCameraDirection();
}

void Camera::processKeyboard(Movement dir, float dt) {
	float speed = movementSpeed * dt;
	glm::vec3 direction = glm::vec3(0.0f);

	switch (dir) {
		case FORWARD:
			direction -= front; // front is -z
			break;
		case BACKWARD:
			direction += front;
			break;
		case RIGHT:
			direction += right;
			break;
		case LEFT:
			direction -= right;
			break;
	}
	//speed * direction = destination vector/velocity
	position += glm::normalize(direction) * speed;
	calcuateViewMatrix();
}

void Camera::processMouseMovement(float xoffset, float yoffset, float dt) {
	float speed = (mouseSensitivity * dt);

	rotation.y += xoffset * speed;
	rotation.x += yoffset * speed;

	// limit pitch to avoid unwanted rotation
	if (rotation.x > 89.0f)
		rotation.x = 89.0f;
	if (rotation.x < -89.0f)
		rotation.x = -89.0f;


	updateCameraDirection();
}
