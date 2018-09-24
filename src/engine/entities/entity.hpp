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

protected:
	// uniform scale
	float scale;

public:
	Entity(std::shared_ptr<Model> model, const glm::vec3& position = glm::vec3(0.0f),
		const glm::vec3& rotation = glm::vec3(0.0f), float scale = 1.0f);

	virtual void getTransMatrix(glm::mat4& mat);

	void increasePosition(float dx, float dy, float dz) {
		position.x += dx;
		position.y += dy;
		position.z += dz;
	}

	void increaseRotation(float dx, float dy, float dz) {
		rotation.x += dx;
		rotation.y += dy;
		rotation.z += dz;
	}

	void setModel(std::shared_ptr<Model> model) {
		this->model = model;
	}

	Model* getModel() const{
		return model.get();
	}

	virtual void setScale(float s) {
		scale = s;
	}

	virtual float getScale() const {
		return scale;
	}
};

#endif