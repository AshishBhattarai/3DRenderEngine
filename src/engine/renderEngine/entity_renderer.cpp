#include "entity_renderer.hpp"

#include "model/model.hpp"

EntityRenderer::EntityRenderer(EntityShader& entityShader, ColoredEntityShader& coloredEntityShader, BoundingBoxShader& boundingBoxShader) :
	entityShader(&entityShader),
	coloredEntityShader(&coloredEntityShader),
	boundingBoxShader(&boundingBoxShader)
{}

void EntityRenderer::loadTransformation(Entity* entity, Shader* shader) {
	// transformation
	glm::mat4 transform_mat;
	entity->getTransMatrix(transform_mat);

	// load to shader
	shader->loadTransformMatrix(transform_mat);
	shader->loadNormalMatrix(glm::mat3(glm::transpose(glm::inverse(transform_mat))));
}

void EntityRenderer::prepareMesh(const TexturedMesh* mesh) {
	// load textures & bind
	entityShader->loadMaterialShininess(mesh->getSpecularShininess());

	glActiveTexture(GL_TEXTURE0+entityShader->DIFFUSE_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID(Texture::DIFFUSE_MAP));

	glActiveTexture(GL_TEXTURE0+entityShader->SPECULAR_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID(Texture::SPECULAR_MAP));

	glActiveTexture(GL_TEXTURE0+entityShader->EMISSION_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID(Texture::EMISSION_MAP));

	glBindVertexArray(mesh->getVAO());
}

void EntityRenderer::prepareMesh(const MaterialMesh* mesh) {
	// load colors
	coloredEntityShader->loadMaterial(mesh->getMaterial());
	glBindVertexArray(mesh->getVAO());
}

void EntityRenderer::renderTexturedEntity(Entity* entity) {
	// render entites with texture
	entityShader->start();
	auto model = entity->getModel();

	loadTransformation(entity, entityShader);
	for(unsigned int i = 0; i < model->getNumMeshes(); ++i) {
		auto mesh = model->getTexturedMesh(i);
		prepareMesh(mesh);
		glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
}
void EntityRenderer::renderColoredEntity(Entity* entity) {
	// render entites with material color
	coloredEntityShader->start();
	auto model = entity->getModel();

	loadTransformation(entity, coloredEntityShader);
	for(unsigned int i = 0; i < model->getNumMeshes(); ++i) {
		auto mesh = model->getMaterialMesh(i);
		prepareMesh(mesh);
		glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
}
void EntityRenderer::renderBoundingBox(Entity* entity) {
	// update aabb mesh if non-static entity
	if(entity->getFlags() & ~EntityFlags::STATIC)
		entity->updateAABB();

	// disable writing to frame buffer
	glDepthMask(GL_FALSE);
	// glDisable(GL_CULL_FACE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// draw bounding box
	boundingBoxShader->start();
	AABBMesh* mesh = entity->getAABBMesh();
	glBindVertexArray(mesh->getVAO());
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	// enable writing
	glDepthMask(GL_TRUE);
	// glEnable(GL_CULL_FACE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

// batch render
void EntityRenderer::render(std::vector<Entity*>& entities) {
	for(auto& entity : entities) {
		auto query = entity->getOcclusionQuery(); // null for objects which are never occluded
		auto model = entity->getModel();

		// visible last frame?
		bool visible = (query)? query->getResult(): true;

		if(query) // if the entity is OCCLUDEE
			query->start();

		if(visible) {
			if(model->isTextured())
				renderTexturedEntity(entity);
			else
				renderColoredEntity(entity);
		} else {
			renderBoundingBox(entity);
		}

		if(query)
			query->end();
	}
}