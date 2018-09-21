#include "terrain_shader.hpp"

void TerrainShader::loadTextureUnits() {
	loadInt("material.texture_blend", BLEND_UNIT);
	loadInt("material.texture_red_diffuse", 	DIFFUSE_RED_UNIT);
	loadInt("material.texture_green_diffuse", DIFFUSE_GREEN_UNIT);
	loadInt("material.texture_blue_diffuse", 	DIFFUSE_BLUE_UNIT);
	loadInt("material.texture_black_diffuse", DIFFUSE_BLACK_UNIT);
}

void TerrainShader::getUniformLocations() {
	// matrices
	loc_transformMat = getUniformLoc("transform_mat");
	loc_normalMat = getUniformLoc("normal_mat");

	loc_materialShininess = getUniformLoc("material.shininess");
}

TerrainShader::TerrainShader():
		Shader(VERTEX_FILE, FRAGMENT_FILE) {
	this->start();
	getUniformLocations();
	loadTextureUnits();
	this->stop();
}