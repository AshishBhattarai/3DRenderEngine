#include "terrain_renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void TerrainRenderer::loadTransformation(Terrain& terrain) {
	// translate
	glm::mat4 transform_mat;
	terrain.getTransMatrix(transform_mat);

	// load to shader
	shader->loadTransformMatrix(transform_mat);
	shader->loadNormalMatrix(glm::mat3(glm::transpose(glm::inverse(transform_mat))));
}

void TerrainRenderer::prepareMesh(const TexturedMesh& mesh) {
	// load textures & bind
	shader->loadMaterialShininess(mesh.getSpecularShininess());

	// blend map
	glActiveTexture(GL_TEXTURE0+TerrainShader::BLEND_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::BLEND_MAP));
	// red
	glActiveTexture(GL_TEXTURE0+TerrainShader::DIFFUSE_RED_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::DIFFUSE_RED_MAP));
	// green
	glActiveTexture(GL_TEXTURE0+TerrainShader::DIFFUSE_GREEN_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::DIFFUSE_GREEN_MAP));
	// blue
	glActiveTexture(GL_TEXTURE0+TerrainShader::DIFFUSE_BLUE_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::DIFFUSE_BLUE_MAP));
	// black
	glActiveTexture(GL_TEXTURE0+TerrainShader::DIFFUSE_BLACK_UNIT);
	glBindTexture(GL_TEXTURE_2D, mesh.getTextureID(Texture::DIFFUSE_BLACK_MAP));

	glBindVertexArray(mesh.getVAO());
}

TerrainRenderer::TerrainRenderer(TerrainShader& shader) :
		shader(&shader)
{}

void TerrainRenderer::render(std::vector<Terrain*>& terrains) {
	shader->start();
	for(auto& terrain : terrains) {
		auto mesh = terrain->getMesh();
		prepareMesh(*mesh);
		loadTransformation(*terrain);
		glDrawElements(GL_TRIANGLE_STRIP, mesh->getIndicesCount(), GL_UNSIGNED_INT, 0);
	}
	shader->stop();
}