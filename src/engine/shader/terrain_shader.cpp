#include "terrain_shader.hpp"

void TerrainShader::getUniformLocations() {
	// matrices
	loc_transformMat = getUniformLoc("transform_mat");
	loc_normalMat = getUniformLoc("normal_mat");

	loc_materialShine = getUniformLoc("material.shininess");
	loc_materialSpecular = getUniformLoc("material.specularFactor");

	// fog
	loc_fogDensity = getUniformLoc("fogDensity");
	loc_fogGradient = getUniformLoc("fogGradient");

	// set texture units
	loadInt("material.texture_blend", BLEND_UNIT);
	loadInt("material.texture_red", 	RED_UNIT);
	loadInt("material.texture_green", GREEN_UNIT);
	loadInt("material.texture_blue", 	BLUE_UNIT);
	loadInt("material.texture_black", BLACK_UNIT);
}

TerrainShader::TerrainShader():
		Shader(VERTEX_FILE, FRAGMENT_FILE) {
	this->start();
	getUniformLocations();
	this->stop();
}