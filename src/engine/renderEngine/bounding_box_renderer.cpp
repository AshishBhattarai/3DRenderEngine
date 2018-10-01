#include "bounding_box_renderer.hpp"

#include <glad/glad.h>

BoundingBoxRenderer::BoundingBoxRenderer(BoundingBoxShader& shader) :
	shader(shader)
{}

void BoundingBoxRenderer::render(std::vector<Entity*>& entities) {
	shader.start();
	for(auto& entity : entities) {
		// check query
		auto query = entity->getOcclusionQuery();
		bool visible = query->getLastResult();

		// render aabb only if not visiable
		if(visible) continue;

		query->start();
		// darw aabb
		AABBMesh* mesh = entity->getAABBMesh();
		glBindVertexArray(mesh->getVAO());
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

		query->end();
	}
}