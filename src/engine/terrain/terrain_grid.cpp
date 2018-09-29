#include "terrain_grid.hpp"

TerrainGrid::	TerrainGrid(u_int numGridX, u_int numGridZ, int size,
		int vertexCount, float maxHeight, int tilingFactor) :
		grid(numGridX*numGridZ),
		numGridX(numGridX),
		numGridZ(numGridZ),
		terSize(size),
		vertexCount(vertexCount),
		maxHeight(maxHeight),
		tilingFactor(tilingFactor),
		numTerrain(0)
{
}

void TerrainGrid::addTerrain(u_int gridX, u_int gridZ, Texture::Map& textures) {
	// check if the index is correct
	if(gridX >= numGridX || gridZ >= numGridZ) return;

	// add PhysicsTerrain
	grid[gridX + numGridZ*gridZ] = std::make_unique<PhysicsTerrain>(
		gridX, gridZ, terSize, vertexCount, textures
	);
	++numTerrain;
}

// with height maps
void TerrainGrid::addTerrain(u_int gridX, u_int gridZ, Image& heightMap,
	Texture::Map& textures)
{
	if(gridX >= numGridX || gridZ >= numGridZ) return;

	// add PhysicsTerrain
	grid[gridX + numGridZ*gridZ] = std::make_unique<PhysicsTerrain>(
		gridX, gridZ, terSize, maxHeight, heightMap, textures
	);
	++numTerrain;
}

// get PhysicsTerrain in grid x, z
PhysicsTerrain* TerrainGrid::getTerrain(u_int gridX, u_int gridZ) {
	if(!numTerrain) return nullptr;
	return (gridX < numGridX)? ((gridZ < numGridZ)?
					grid[gridX + numGridZ*gridZ].get() : nullptr) : nullptr;
}

// get grid for correspoind world x, z
std::pair<u_int, u_int> TerrainGrid::posToGrid(float x, float z) {
	u_int gridX = (u_int)(x/terSize);
	u_int gridZ = (u_int)(z/terSize);
	if(!numTerrain || gridX >= numGridX || gridZ >= numGridZ)
		return std::make_pair(0, 0);
	return std::make_pair(gridX, gridZ);
}

// get height at position x, y (world position)
float TerrainGrid::getHeightAt(float x, float z) {
	std::pair<u_int, u_int> g = posToGrid(x, z);
	return grid[g.first + g.second*numGridZ]->getTerrainHeight(x, z);
}
