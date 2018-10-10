#include "terrain_grid.hpp"

TerrainGrid::	TerrainGrid(unsigned int numGridX, unsigned int numGridZ, int size,
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

void TerrainGrid::addTerrain(unsigned int gridX, unsigned int gridZ, Texture::Map& textures) {
	// check if the index is correct
	if(gridX >= numGridX || gridZ >= numGridZ) return;

	// add PhysicsTerrain
	grid[gridX + numGridZ*gridZ] = std::make_unique<PhysicsTerrain>(
		gridX, gridZ, terSize, vertexCount, textures
	);
	++numTerrain;
}

// with height maps
void TerrainGrid::addTerrain(unsigned int gridX, unsigned int gridZ, Image& heightMap,
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
PhysicsTerrain* TerrainGrid::getTerrain(unsigned int gridX, unsigned int gridZ) {
	if(!numTerrain) return nullptr;
	return (gridX < numGridX)? ((gridZ < numGridZ)?
					grid[gridX + numGridZ*gridZ].get() : nullptr) : nullptr;
}

// get grid for correspoind world x, z
std::pair<unsigned int, unsigned int> TerrainGrid::posToGrid(float x, float z) {
	unsigned int gridX = (unsigned int)(x/terSize);
	unsigned int gridZ = (unsigned int)(z/terSize);
	if(!numTerrain || gridX >= numGridX || gridZ >= numGridZ)
		return std::make_pair(0, 0);
	return std::make_pair(gridX, gridZ);
}

// get height at position x, y (world position)
float TerrainGrid::getHeightAt(float x, float z) {
	std::pair<unsigned int, unsigned int> g = posToGrid(x, z);
	return grid[g.first + g.second*numGridZ]->getTerrainHeight(x, z);
}
