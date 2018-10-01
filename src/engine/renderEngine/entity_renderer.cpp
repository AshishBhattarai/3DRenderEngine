#include "entity_renderer.hpp"

#include "model/model.hpp"

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

	glActiveTexture(GL_TEXTURE0+entityShader->DIFFUSE_UINT);
	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID(Texture::DIFFUSE_MAP));

	glActiveTexture(GL_TEXTURE0+entityShader->SPECULAR_UINT);
	glBindTexture(GL_TEXTURE_2D, mesh->getTextureID(Texture::SPECULAR_MAP));
	glBindVertexArray(mesh->getVAO());
}

void EntityRenderer::prepareMesh(const MaterialMesh* mesh) {
	// load colors
	coloredEntityShader->loadMaterial(mesh->getMaterial());
	glBindVertexArray(mesh->getVAO());
}

EntityRenderer::EntityRenderer(EntityShader& entityShader, ColoredEntityShader& coloredEntityShader) :
	entityShader(&entityShader),
	coloredEntityShader(&coloredEntityShader)
{}

// batch render
void EntityRenderer::render(std::vector<Entity*>& entities) {
	for(auto& entity : entities) {
		// query
		auto query = entity->getOcclusionQuery();

		// check model type
		auto model = entity->getModel();
		bool textured = (model->getModelType() == Model::MATERIAL_TEXTURE);
		Shader* shader = (textured)? static_cast<Shader*>(entityShader) : static_cast<Shader*>(coloredEntityShader);
		shader->start();

		query->start();
		// render mehes
		loadTransformation(entity, shader);
		for(unsigned i = 0; i < model->getnumMeshes(); ++i) {
			if(textured)
				prepareMesh(model->getTexturedMesh(i));
			else
				prepareMesh(model->getMaterialMesh(i));

			glDrawElements(GL_TRIANGLES, model->getRawMesh(i)->getIndicesCount(), GL_UNSIGNED_INT, 0);
		}
		query->end();
	}
}