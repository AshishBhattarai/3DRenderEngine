#ifndef TERRAIN_GRID_HPP
#define TERRAIN_GRID_HPP

#include <memory>
#include <utility>

#include "terrain.hpp"

// represents a grid of Terrains

class TerrainGrid {
private:
	using Grid = std::vector<std::vector<std::shared_ptr<Terrain>>>;
	// defualts
	static constexpr int SIZE = 1000;
	static constexpr int VERTEX_COUNT = 256;
	static constexpr int TILING_FACTOR = 50;
	static constexpr float MAX_HEIGHT = 50;

	int terSize; // Terrain size
	int vertexCount; // total num of vertex
	float maxHeight;
	int tilingFactor; // texture tiling factor - no. of times the texture gets tiled

	float colOffset; // collision(height map) offset

	// Terrain grid
	Grid grid;
	int numTerrain; // no. of terrins in grid
	u_int numGridX; // total no. of Terrain in grid x, starts from 1
	u_int numGridZ; // in z

public:
	TerrainGrid(u_int numGridX, u_int numGridZ, int size = SIZE,
		int vertexCount = VERTEX_COUNT, float maxHeight = MAX_HEIGHT,
		int tilingFactor = TILING_FACTOR);

	// add a Terrain to the grid - overrides old Terrain if there is any
	void addTerrain(u_int gridX, u_int gridZ, Texture::Map textures);
	// Terrains with heightmap
	void addTerrain(u_int gridX, u_int gridZ, Image& heightMap, Texture::Map textures);

	// get Terrain in grid x, z
	Terrain* getTerrain(u_int gridX, u_int gridZ);

	// get grid for correspoind world x, z, returns 0 if no Terrain in gri
	std::pair<u_int, u_int> posToGrid(float x, float z);

	// get height at position x, y (world position)
	float getHeightAt(float x, float z);

	// getters
	bool hasTerrain() const {
		return numTerrain;
	}

	bool hasTerrainAt(u_int x, u_int z) const {
		return grid[x][z].get();
	}

	float getTerrainSize() const {
		return terSize;
	}

	float getTerrainMaxHeight() const {
		return maxHeight;
	}

	float getMaxHeight() const {
		return maxHeight;
	}

	float getTerrainVertexCount() const {
		return vertexCount;
	}

	float getColOffset() const {
		return colOffset;
	}

	float getNumGridX() const {
		return numGridX;
	}

	float getNumGridZ() const {
		return numGridZ;
	}

	int getNumTerrain() const {
		return numTerrain;
	}
};

#endif