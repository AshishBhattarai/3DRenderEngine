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
	// mesh
	std::unique_ptr<TexturedMesh> mesh;
	Mode mode;

protected:
	u_int vertexCount;
	int size;
	// world position
	float posX;
	float posZ;
	// size per Terrain suqare grid
	float gridSize;
	// heights
	float maxHeight;
	std::vector<float> heights;

	float calcHeightAt(u_int x, u_int z, Image& heightMap);
	glm::vec3 calcNormalAt(u_int x, u_int z, Image& heightMap);

	// generates the Terrain mesh -
	void generateVertices(std::vector<TexturedMesh::Vertex>& vertices, std::vector<glm::vec2>& texCoords);
	void generateIndices(std::vector<u_int>& indices);
	void generateHeightWMap(std::vector<TexturedMesh::Vertex>& vertices, Image& image);

	std::unique_ptr<TexturedMesh> generateTerrain(std::function<void(std::vector<TexturedMesh::Vertex>&)> caculateHeight = {});
		// function to calculate Terrain height (empty by default)

	// intilizing constructor
	Terrain(u_int gridX, u_int gridZ, int size, u_int vertexCount, float maxHeight, Mode mode);

public:
	// flat Terrain constructor
	Terrain(u_int gridX, u_int gridZ, int size, u_int vertexCount, Texture::Map& textures);
	// height map constructor
	Terrain(u_int gridX, u_int gridZ, int size, float maxHeight, Image& heightMap,
	Texture::Map& textures);

	// get height at a world position
	float getTerrainHeight(float posX, float posZ);

	void getTransMatrix(glm::mat4& trans);

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