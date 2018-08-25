#include "entity_renderer.hpp"

#include "model/model.hpp"

void EntityRenderer::loadTransformation(const Entity& entity) {
	// translate
	glm::mat4 transform_mat = glm::translate(glm::mat4(1.0f), entity.getPosition());
	// rotate
	transform_mat = glm::rotate(transform_mat, glm::radians(entity.getPitch()),
			glm::vec3(1.0f, 0.0f, 0.0f));
	transform_mat = glm::rotate(transform_mat, glm::radians(entity.getYaw()),
			glm::vec3(0.0f, 1.0f, 0.0f));
	transform_mat = glm::rotate(transform_mat, glm::radians(entity.getRoll()),
			glm::vec3(0.0f, 0.0f, 1.0f));
	// scale
	transform_mat = glm::scale(transform_mat, glm::vec3(entity.getScale()));

	// load to shader
	shader->loadTransformMatrix(transform_mat);
	shader->loadNormalMatrix(glm::mat3(glm::transpose(glm::inverse(transform_mat))));
}

void EntityRenderer::prepareMesh(const TexturedMesh& mesh) {
	// load textures & bind
	shader->loadMaterialSpecular(mesh.getSpecularFactor(), mesh.getSpecularShine());
	shader->loadHasSpecular(mesh.getTextureID(Texture::SPECULAR_MAP));

	if(!mesh.getIsCullFace())
		glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0+shader->DIFFUSE_UINT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::DIFFUSE_MAP));

	glActiveTexture(GL_TEXTURE0+shader->SPECULAR_UINT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::SPECULAR_MAP));
	glBindVertexArray(mesh.getVAO());
}

EntityRenderer::EntityRenderer(EntityShader& shader) :
		shader(&shader)
{}

// batch render
void EntityRenderer::render(EntityListMap& entities) {
	// loop through all the map pairs
	for(auto& pair : entities) {
		 // loop through all the meshes
		for(int i = 0; i < pair.first->getnumMeshes(); ++i) {
			auto mesh = pair.first->getTexturedMesh(i);
			prepareMesh(*mesh); // load texture, VAO
			// entites
			for(auto& entity : pair.second) {
				loadTransformation(*entity); // load transformation
				// draw
				glDrawElements(GL_TRIANGLES, mesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
			}
		}
	}	// end map loop
}