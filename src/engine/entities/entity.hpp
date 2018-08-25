#ifndef ENTITY_H
#define ENTITY_H

#include "base_entity.hpp"

#include <glm/glm.hpp>
#include <memory>

#include "model/model.hpp"

// Represent the entities which will be rendered

class Entity : public BaseEntity {
private:
	std::shared_ptr<Model> model;
	glm::vec3 position;

	// uniform scale
	float scale;

public:
	Entity(std::shared_ptr<Model> model, glm::vec3 position = glm::vec3(0.0f),
		float pitch = 0.0f, float yaw = 0.0f, float roll = 0.0f, float scale = 1.0f);

	void increasePosition(float dx, float dy, float dz) {
		position.x += dx;
		position.y += dy;
		position.z += dz;
	}

	void increaseRotation(float dx, float dy, float dz) {
		pitch += dx;
		yaw += dy;
		roll += dz;
	}

	void setModel(std::shared_ptr<Model> model) {
		this->model = model;
	}

	Model* getModel() const{
		return model.get();
	}

	void setScale(float s) {
		scale = s;
	}

	float getScale() const {
		return scale;
	}
};

#endif