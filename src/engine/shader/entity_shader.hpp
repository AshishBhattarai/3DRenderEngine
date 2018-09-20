#ifndef ENTITY_SHADER_HPP
#define ENTITY_SHADER_HPP

#include "shader.hpp"

#include <string_view>

#include "entities/light.hpp"
#include "model/mesh.hpp"

// Shader for entities in the world

class EntityShader : public Shader {
public:
	// texture uints for texture
	static constexpr int DIFFUSE_UINT  = 0;
	static constexpr int SPECULAR_UINT = 1;

private:
	static constexpr std::string_view VERTEX_FILE = "shaders/entity.vert";
	static constexpr std::string_view FRAGMENT_FILE = "shaders/entity.frag";

	// materices
	GLint loc_transformMat;
	GLint loc_normalMat;

	// material
	GLint loc_materialAmbient;
	GLint loc_materialDiffuse;
	GLint loc_materialShininess;

	// get all the locations from the shaders
	void getUniformLocations();

public:
	EntityShader();

	// load uniforms
	// matrices
	void loadTransformMatrix(const glm::mat4& matrix) {
		uniformMatrix4fv(loc_transformMat, matrix);
	}

	void loadNormalMatrix(const glm::mat3& matrix) {
		uniformMatrix3fv(loc_normalMat, matrix);
	}

	// specular
	void loadMaterialShininess(float shininess) {
		uniform1f(loc_materialShininess, shininess);
	}
};

#endif