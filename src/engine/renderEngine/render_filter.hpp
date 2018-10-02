#ifndef RENDER_FILTER_HPP
#define RENDER_FILTER_HPP

#include <unordered_map>
#include <vector>
#include <bullet/BulletCollision/BroadphaseCollision/btDbvt.h>

#include "entities/entity.hpp"
#include "terrain/terrain.hpp"

// stores renderObject with its type terrain or entity
struct RenderObject {
	enum Type {
		ENTITY,
		TERRAIN
	};

	void* data;
	Type type;
};

// ICollide::Process is called on collision of frustum plane and objs aabb
class RenderFilter : public btDbvt::ICollide {
private:
	std::vector<Entity*>& entities;
	std::vector<Terrain*>& terrains;

public:
	RenderFilter(std::vector<Entity*>& entities, std::vector<Terrain*>& terrains);

	void Process(const btDbvtNode* node, btScalar scalar);
	void Process(const btDbvtNode* node);
};

#endif