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

void EntityRenderer::prepareMesh(const TexturedMesh& mesh) {
	// load textures & bind
	entityShader->loadMaterialShininess(mesh.getSpecularShininess());

	glActiveTexture(GL_TEXTURE0+entityShader->DIFFUSE_UINT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::DIFFUSE_MAP));

	glActiveTexture(GL_TEXTURE0+entityShader->SPECULAR_UINT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::SPECULAR_MAP));
	glBindVertexArray(mesh.getVAO());
}

void EntityRenderer::prepareMesh(const MaterialMesh& mesh) {
	// load colors
	coloredEntityShader->loadMaterial(mesh.getMaterial());
	glBindVertexArray(mesh.getVAO());
}

EntityRenderer::EntityRenderer(EntityShader& entityShader, ColoredEntityShader& coloredEntityShader) :
	entityShader(&entityShader),
	coloredEntityShader(&coloredEntityShader)
{}

// batch render
void EntityRenderer::render(EntityListMap& entities) {
	// loop through all the map pairs
	for(auto& pair : entities) {
		// check type
		bool textured = (pair.first->getModelType() == Model::MATERIAL_TEXTURE);
		Shader* shader = (textured)?
			static_cast<Shader*>(entityShader) :
			static_cast<Shader*>(coloredEntityShader);
		// start shader
		shader->start();
		// loop through all the meshes
		for(u_int i = 0; i < pair.first->getnumMeshes(); ++i) {
			if(textured)
				prepareMesh(*pair.first->getTexturedMesh(i));
			else
				prepareMesh(*pair.first->getMaterialMesh(i));
			// get mesh data
			const Mesh* mesh = pair.first->getRawMesh(i);
			// entites
			for(auto& entity : pair.second) {
				loadTransformation(entity, shader); // load transformation
				// draw
				glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
			}
		}
		shader->stop();
	}	// end map loop
}