#include "entity_shader.hpp"

void EntityShader::loadTextureUnits() {
	loadInt("material.texture_diffuse", DIFFUSE_UNIT);
	loadInt("material.texture_specular", SPECULAR_UNIT);
	loadInt("material.texture_emission", EMISSION_UNIT);
}

void EntityShader::getUniformLocations() {
	// matrices
	loc_transformMat = getUniformLoc("transform_mat");
	loc_normalMat = getUniformLoc("normal_mat");

	// material
	loc_materialShininess = getUniformLoc("material.shininess");
}

EntityShader::EntityShader():
		Shader(VERTEX_FILE, FRAGMENT_FILE) {
	this->start();
	getUniformLocations();
	loadTextureUnits();
	this->stop();
}