#ifndef TERRAIN_SHADER_HPP
#define TERRAIN_SHADER_HPP

#include "shader.hpp"

#include <string_view>

#include "entities/light.hpp"
#include "model/mesh.hpp"

// Shader for terrains

class TerrainShader : public Shader {
public:
	// texture UNITs for texture
	static constexpr int BLEND_UNIT = 0;
	static constexpr int DIFFUSE_RED_UNIT 	= 1;
	static constexpr int DIFFUSE_GREEN_UNIT = 2;
	static constexpr int DIFFUSE_BLUE_UNIT	= 3;
	static constexpr int DIFFUSE_BLACK_UNIT = 4;

private:
	static constexpr std::string_view VERTEX_FILE = "shaders/entity.vert"; // same as entity shader
	static constexpr std::string_view FRAGMENT_FILE = "shaders/terrain.frag";

	// materices
	GLint loc_transformMat;
	GLint loc_normalMat;

	// fog
	GLint loc_fogDensity;
	GLint loc_fogGradient;

	// material
	GLint loc_materialShininess;

	// get all the locations from the shaders
	void getUniformLocations();

public:
	TerrainShader();

	// load uniforms
	void loadTransformMatrix(const glm::mat4& matrix) {
		uniformMatrix4fv(loc_transformMat, matrix);
	}

	void loadNormalMatrix(const glm::mat3& matrix) {
		uniformMatrix3fv(loc_normalMat, matrix);
	}

	// material
	void loadMaterialShininess(float shininess) {
		uniform1f(loc_materialShininess, shininess);
	}
};

#endif