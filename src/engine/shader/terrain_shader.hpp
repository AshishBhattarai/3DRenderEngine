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
	static constexpr int RED_UNIT 	= 1;
	static constexpr int GREEN_UNIT = 2;
	static constexpr int BLUE_UNIT	= 3;
	static constexpr int BLACK_UNIT = 4;

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
	GLint loc_materialSpecular;
	GLint loc_materialShine;

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
	void loadMaterialSpecular(float factor, float shine) {
		uniform1f(loc_materialShine, shine);
		uniform1f(loc_materialSpecular, factor);
		// load factor
	}
};

#endif