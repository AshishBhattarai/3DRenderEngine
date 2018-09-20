#include "entity_shader.hpp"

void EntityShader::getUniformLocations() {
	// matrices
	loc_transformMat = getUniformLoc("transform_mat");
	loc_normalMat = getUniformLoc("normal_mat");

	// material
	loc_materialShininess = getUniformLoc("material.shininess");

	// set texture units
	loadInt("material.texture_diffuse", DIFFUSE_UINT);
	loadInt("material.texture_specular", SPECULAR_UINT);
}

EntityShader::EntityShader():
		Shader(VERTEX_FILE, FRAGMENT_FILE) {
	this->start();
	getUniformLocations();
	this->stop();
}