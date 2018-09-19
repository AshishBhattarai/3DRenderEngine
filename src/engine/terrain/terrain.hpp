#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include <functional>
#include <memory>

#include "model/textured_mesh.hpp"
#include "utils/image.hpp"

class Terrain {
public:
	// Terrain Mode
	enum Mode {
		FLAT				=		00,
		HEIGHT_MAP	=		01
	};

private:
	static constexpr float MAX_PIXEL_COLOR = 256 + 256 + 256;// max color val of the pixel = r + g + b

	int vertexCount;
	// world position
	float posX;
	float posZ;
	// size per Terrain suqare grid
	float gridSize;
	// heights
	std::vector<float> heights;

	// mesh
	std::unique_ptr<TexturedMesh> mesh;

	Mode mode;

	float calcHeightAt(int x, int z, float max_Height, Image& heightMap);
	glm::vec3 calcNormalAt(int x, int z, float max_Height, Image& heightMap);

	// generates the Terrain mesh -
	void generateVertices(int size, std::vector<TexturedMesh::Vertex>& vertices, std::vector<glm::vec2>& texCoords);
	void generateIndices(std::vector<u_int>& indices);
	void generateHeightWMap(std::vector<TexturedMesh::Vertex>& vertices, float maxHeight, Image& image);

	std::unique_ptr<TexturedMesh> generateTerrain(int size,
		std::function<void(std::vector<TexturedMesh::Vertex>&)> caculateHeight = {});
		// function to calculate Terrain height (empty by default)

	// intilizing constructor
	Terrain(u_int gridX, u_int gridZ, int size, int vertexCount, Mode mode);

public:
	// flat Terrain constructor
	Terrain(u_int gridX, u_int gridZ, int size, int vertexCount, Texture::Map textures);
	// height map constructor
	Terrain(u_int gridX, u_int gridZ, int size, float maxHeight, Image& heightMap,
	Texture::Map textures);

	// get height at a world position
	float getTerrainHeight(float posX, float posZ);

	// getters
	float getPosX() const {
		return posX;
	}

	float getPosZ() const {
		return posZ;
	}

	const TexturedMesh* getMesh() const {
		return mesh.get();
	}

	const std::vector<float>& getHeights() const {
		return heights;
	}

	// setter
	void setSpecularShininess(float shininess) {
		mesh->setSpecularShininess(shininess);
	}

	float setSpecularShininess() {
		return mesh->getSpecularShininess();
	}

};

#endif