#ifndef ENTITY_H
#define ENTITY_H

#include "base_entity.hpp"

#include <glm/glm.hpp>
#include <memory>

#include "model/model.hpp"
#include "model/aabb_mesh.hpp"
#include "renderEngine/opengl_query.hpp"
#include "shape/aabb.hpp"

// Represent the entities which will be rendered

namespace EntityFlags {
	constexpr int PHYSICS 	=	 	01;
	constexpr int STATIC		=		02;
	constexpr int OCCLUDEE  =		04;
}

class Entity : public BaseEntity {
public:
	enum Type {
		BASIC = 0,
		LAMP	=	1
	};

private:
	std::shared_ptr<Model> model;
	glm::vec3 minBB;
	glm::vec3 maxBB;

	std::unique_ptr<OpenglQuery> occu_query;

	void applyRotationBB();
	void applyTranslationBB();
	void applyScaleBB();

protected:
	// uniform scale
	float scale;
	AABB curr_aabb; // current aabb
	AABB last_aabb; // last aabb - before current transformation
	AABBMesh aabb_mesh;
	int flags;
	Type type;


	Entity(std::shared_ptr<Model> model, const glm::vec3& position,
		const glm::vec3& rotation, float scale, int flags, Type type = BASIC);

public:
	Entity(std::shared_ptr<Model> model, const glm::vec3& position = glm::vec3(0.0f),
		const glm::vec3& rotation = glm::vec3(0.0f), float scale = 1.0f);
	virtual ~Entity();

	virtual void getTransMatrix(glm::mat4& mat);
	virtual void updateAABB();
	void makeOccludee(bool make = true);

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

	virtual void setScale(float s) {
		scale = s;
	}

	virtual float getScale() const {
		return scale;
	}

	const Model* getModel() const {
		return model.get();
	}

	virtual glm::vec3 getMinBB() const {
		return curr_aabb.getMinBB();
	}

	virtual glm::vec3 getMaxBB() const {
		return curr_aabb.getMaxBB();
	}

	virtual AABB getAABB() const {
		return curr_aabb;
	}

	AABBMesh* getAABBMesh() {
		return &aabb_mesh;
	}

	OpenglQuery* getOcclusionQuery() {
		return occu_query.get();
	}

	int getFlags() const {
		return flags;
	}

	bool isStatic() const {
		return (flags & EntityFlags::STATIC);
	}

	bool isPhysics() const {
		return (flags & EntityFlags::PHYSICS);
	}

	Type getType() const {
		return type;
	}
};

#endif