#ifndef COLORED_ENTITY_SHADER_HPP
#define COLORED_ENTITY_SHADER_HPP

#include "shader.hpp"

#include <string_view>

#include "model/material_mesh.hpp"

class ColoredEntityShader : public Shader {
private:
	static constexpr std::string_view VERTEX_FILE = "shaders/colored_entity.vert";
	static constexpr std::string_view FRAGMENT_FILE = "shaders/colored_entity.frag";

	GLint loc_transformMat;
	GLint loc_normalMat;

	GLint loc_materialDiffuse;
	GLint loc_materialSpecular;
	GLint loc_materialShininess;

	void getUniformLocations();

public:
	ColoredEntityShader();

	void loadTransformMatrix(const glm::mat4& matrix) {
		uniformMatrix4fv(loc_transformMat, matrix);
	}

	void loadNormalMatrix(const glm::mat3& matrix) {
		uniformMatrix3fv(loc_normalMat, matrix);
	}

	void loadMaterial(const MaterialMesh::Material& material) {
		uniform4fv(loc_materialDiffuse, glm::vec4(material.diffuse, 0.0f));
		uniform4fv(loc_materialSpecular, glm::vec4(material.specular, 0.0f));
		uniform1f(loc_materialShininess, material.shininess);
	}
};

#endif