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
	GLint loc_materialSpecular;
	GLint loc_materialShine;
	GLint loc_hasSpecular;

	// fog
	GLint loc_fogDensity;
	GLint loc_fogGradient;

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
	void loadMaterialSpecular(float factor, float shine) {
		uniform1f(loc_materialShine, shine);
		uniform1f(loc_materialSpecular, factor);
	}

	void loadHasSpecular(bool val) {
		uniform1i(loc_hasSpecular, val);
	}
};

#endif