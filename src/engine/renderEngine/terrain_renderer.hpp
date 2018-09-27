#ifndef TERRAIN_RENDERER_HPP
#define	TERRAIN_RENDERER_HPP

#include <vector>

#include "shader/terrain_shader.hpp"
#include "terrain/terrain.hpp"

class TerrainRenderer {
private:
	TerrainShader* shader;

	void loadTransformation(Terrain& terrain);
	void prepareMesh(const TexturedMesh& mesh);
	// void loadTransformation(const Terrain& Terrain);

public:
	TerrainRenderer(TerrainShader& shader);

	// render a list of Terrains
	void render(std::vector<Terrain*>& terrains);
};

#endif