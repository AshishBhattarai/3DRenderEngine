#include "colored_entity_shader.hpp"

void ColoredEntityShader::getUniformLocations() {
	loc_transformMat = getUniformLoc("transform_mat");
	loc_normalMat = getUniformLoc("normal_mat");

	loc_materialAmbient = getUniformLoc("material.color.ambient");
	loc_materialDiffuse = getUniformLoc("material.color.diffuse");
	loc_materialSpecular = getUniformLoc("material.color.specular");
	loc_materialShininess = getUniformLoc("material.shininess");
}

ColoredEntityShader::ColoredEntityShader():
	Shader(VERTEX_FILE, FRAGMENT_FILE)
{
	this->start();
	getUniformLocations();
	this->stop();
}