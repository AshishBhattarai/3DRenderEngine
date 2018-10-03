#include "render_filter.hpp"

RenderFilter::RenderFilter(std::vector<Entity*>& entities, std::vector<Terrain*>& terrains) :
	entities(entities),
	terrains(terrains)
{}

void RenderFilter::Process(const btDbvtNode* node, btScalar) {
	Process(node);
}

void RenderFilter::Process(const btDbvtNode* node) {
		Entity* entity = static_cast<Entity*>(node->data);
		entities.emplace_back(entity);
}