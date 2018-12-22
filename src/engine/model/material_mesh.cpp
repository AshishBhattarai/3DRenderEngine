#include "material_mesh.hpp"

void MaterialMesh::removeGamma() {
	material.diffuse.r = glm::pow(material.diffuse.r, 2.2f);
	material.diffuse.g = glm::pow(material.diffuse.g, 2.2f);
	material.diffuse.b = glm::pow(material.diffuse.b, 2.2f);
}

void MaterialMesh::setMaterial(Material material) {
	this->material = material;
	removeGamma();
}