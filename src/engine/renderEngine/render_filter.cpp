#include "render_filter.hpp"

RenderFilter::RenderFilter(std::unordered_map<Model*, std::vector<Entity*>>& entities,
	std::vector<Terrain*>& terrains) :
	entities(entities),
	terrains(terrains)
{}

void RenderFilter::Process(const btDbvtNode* node, btScalar) {
	Process(node);
}

void RenderFilter::Process(const btDbvtNode* node) {
	RenderObject* obj = static_cast<RenderObject*>(node->data);
	if(obj->type == RenderObject::ENTITY) {
		// entities
		Entity* entity = static_cast<Entity*>(obj->data);
		entities[entity->getModel()].emplace_back(entity);
	} else {
		// terrains
		Terrain* terrain = static_cast<Terrain*>(obj->data);
		terrains.emplace_back(terrain);
	}
	delete obj;
}