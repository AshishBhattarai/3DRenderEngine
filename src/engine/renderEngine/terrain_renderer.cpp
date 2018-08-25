#include "terrain_renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void TerrainRenderer::loadTransformation(const Terrain& terrain) {
	// translate
	glm::mat4 transform_mat = glm::translate(glm::mat4(1.0f), glm::vec3(terrain.getPosX(), 0.0f, terrain.getPosZ()));

	// load to shader
	shader->loadTransformMatrix(transform_mat);
	shader->loadNormalMatrix(glm::mat3(glm::transpose(glm::inverse(transform_mat))));
}

void TerrainRenderer::prepareMesh(const TexturedMesh& mesh) {
	// load textures & bind
	shader->loadMaterialSpecular(mesh.getSpecularFactor(), mesh.getSpecularShine());

	// blend map
	glActiveTexture(GL_TEXTURE0+TerrainShader::BLEND_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::BLEND_MAP));
	// red
	glActiveTexture(GL_TEXTURE0+TerrainShader::RED_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::RED_TEXTURE));
	// green
	glActiveTexture(GL_TEXTURE0+TerrainShader::GREEN_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::GREEN_TEXTURE));
	// blue
	glActiveTexture(GL_TEXTURE0+TerrainShader::BLUE_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::BLUE_TEXTURE));
	// black
	glActiveTexture(GL_TEXTURE0+TerrainShader::BLACK_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::BLACK_TEXTURE));

	glBindVertexArray(mesh.getVAO());
}

TerrainRenderer::TerrainRenderer(TerrainShader& shader) :
		shader(&shader)
{}

void TerrainRenderer::render(std::vector<Terrain*>& terrains) {
	for(auto& terrain : terrains) {
		auto mesh = terrain->getMesh();
		prepareMesh(*mesh);
		loadTransformation(*terrain);
		glDrawElements(GL_TRIANGLE_STRIP, mesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
}