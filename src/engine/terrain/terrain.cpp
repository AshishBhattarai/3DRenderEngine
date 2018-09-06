#include "terrain.hpp"

#include "utils/slogger.hpp"
#include "utils/math.hpp"

float Terrain::calcHeightAt(int x, int z, float maxHeight, Image& heightMap) {
	if ( x < 0 || x >= heightMap.getWidth() || z < 0 || z >= heightMap.getHeight())
		return 0.0f;

	// get pixel color
  float height = heightMap.getRGB(x, z);
	// calc height
	height -= (MAX_PIXEL_COLOR/2.0f); // between -MAX_PIXEL_COLOR/2 and MAX_PIXEL_COLOR/2
	height /= (MAX_PIXEL_COLOR/2.0f);	// between -1 & 1

	height *= maxHeight; // between -MAX_HEIGHT & MAX_HEIGHT
  return height;
}

// calculate normal baised on heights of surrounding(neighbour) vertices, (finite difference method)
glm::vec3 Terrain::calcNormalAt(int x, int z, float maxHeight, Image& heightMap) {
	float heightL = calcHeightAt(x-1, z, maxHeight, heightMap); // left
	float heightR = calcHeightAt(x+1, z, maxHeight, heightMap); // right
	float heightU = calcHeightAt(x, z+1, maxHeight, heightMap); // up
	float heightD = calcHeightAt(x, z-1, maxHeight, heightMap); // down
	// calculate normal
	glm::vec3 normal(heightL-heightR, 2.0f, heightD-heightU);
	return glm::normalize(normal);
}

// generate vertex data
void Terrain::generateVertices(int size,
	std::vector<TexturedMesh::Vertex>& vertices, std::vector<glm::vec2>& texCoords)
{
	for(int z = 0; z < vertexCount; ++z) { // z - h
		for(int x = 0; x < vertexCount; ++x) { // x - w
			Mesh::Vertex vertex;
			glm::vec2 texCoord;
			// position
			vertex.position.x = ((float)x/(vertexCount-1)) * size;
			vertex.position.y = 0.0f;
			vertex.position.z = ((float)z/(vertexCount-1)) * size;
			// normal
			vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			// texture coord
			texCoord.x = (float)x/(vertexCount - 1);
			texCoord.y = (float)z/(vertexCount - 1);
			vertices.emplace_back(vertex);
			texCoords.emplace_back(texCoord);
		}
	}
}

// generate indices data
void Terrain::generateIndices(std::vector<u_int>& indices) {
	// generate indices for tringle strips
	for(int z = 0; z < vertexCount-1; ++z) { // height
		for(int x = 0; x < vertexCount; ++x) { // width
			// indices - in (x, z) for CCW - bottom to top
			indices.push_back(z*(vertexCount) + x); // bottom left vertex
			indices.push_back((z+1)*(vertexCount) + x); // top left vertex
		}
		// create degenerate triangle (except in last row)
		if(z != vertexCount-2) {
			// repeate last of row(this)
			indices.push_back((z+1)*(vertexCount) + (vertexCount-1)); // current bottom
			// repeate first of next row
			indices.push_back((z+1) * (vertexCount)); // next top
		}
	}
}

// generate height with heightMap
void Terrain::generateHeightWMap(std::vector<TexturedMesh::Vertex>& vertices,
	float maxHeight, Image& image)
{
	int i;
	for(int z = 0; z < vertexCount; ++z) {
		for(int x = 0; x < vertexCount; ++x) {
			i = x + z*(vertexCount);
			// calc height
			heights[i] = calcHeightAt(x, z, maxHeight, image);
			//vertex height
			vertices[i].position.y = heights[i];
			// normals
			vertices[i].normal = calcNormalAt(x, z, maxHeight, image);
		}
	}
}

// takes Terrain size, vertex count, and repeate factor for Terrain texture
std::unique_ptr<TexturedMesh> Terrain::generateTerrain(int size,
	std::function<void(std::vector<TexturedMesh::Vertex>&)> generateHeight)
{
	int total_vertices = vertexCount * vertexCount;
	// vectors to store mesh data
	std::vector<Mesh::Vertex> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<u_int> indices;

	// reserve space
	vertices.reserve(total_vertices);
	texCoords.reserve(total_vertices);
	indices.reserve(2*vertexCount*vertexCount);

	// vertices
	generateVertices(size, vertices, texCoords);
	// indices
	generateIndices(indices);
	// height
	if(generateHeight) {
		generateHeight(vertices);
	}

	// create a mesh
	return std::make_unique<TexturedMesh>(vertices, indices, texCoords);
}

// common constrcutor
Terrain::Terrain(u_int gridX, u_int gridZ, int size, int vertexCount, Mode mode) :
	vertexCount(vertexCount),
	posX(gridX*size),
	posZ(gridZ*size),
	mode(mode),
	gridSize((float)size/(vertexCount - 1)), // Total grids per row&col
	heights((vertexCount)*(vertexCount), 0.0f) // reserve
{}

// flat Terrain
Terrain::Terrain(u_int gridX, u_int gridZ, int size, int vertexCount, Texture::Map textures) :
	Terrain(gridX, gridZ, size, vertexCount, FLAT)
{
	mesh = generateTerrain(size);
	mesh->setSpecularProperty(TER_SPECULAR, TER_SHINE);
	mesh->setTextureMap(textures);
}

// height map Terrain
Terrain::Terrain(u_int gridX, u_int gridZ, int size, float maxHeight, Image& heightMap,
	Texture::Map textures) :
	Terrain(gridX, gridZ, size, heightMap.getWidth(), HEIGHT_MAP)
{
	// generateTerrain with heightMap
	mesh = generateTerrain(size,
		[this, &heightMap, &maxHeight](std::vector<TexturedMesh::Vertex>& vertices)
	{
		generateHeightWMap(vertices, maxHeight, heightMap);
	});

	mesh->setSpecularProperty(TER_SPECULAR, TER_SHINE);
	mesh->setTextureMap(textures);
}

float Terrain::getTerrainHeight(float posX, float posZ) {
	float height = 0.0f;
	// get the position in relation to Terrain
	/** If Terrain is scaled or rotated:
	 * glm::vec3 rPos =  glm::inverse(Terrain_modelMat) * glm::vec4(worldPos, 1.0f);
	 **/
	float terX = posX - this->posX;
	float terZ = posZ - this->posZ;
	// get grid(x,y) - index for heights array
	u_int gridX = abs(floor(terX/gridSize));
	u_int gridZ = abs(floor(terZ/gridSize));
	// check if index lies inside the Terrain
	if(gridX >= vertexCount-1 || gridZ >= vertexCount -1)
		return 0.0f;
	// get the position inside the grid square
	float x = fmod(terX, gridSize) / gridSize;
	float z = fmod(terZ, gridSize) / gridSize;

	// get the tringle on which posX & posZ lie
	if(x <= (1 - z)) { // upper triangle
		// calculate height at (x, z) by interpolating know heights
		height = Math::barryCentric(
			glm::vec3(0.0f, heights[gridX + gridZ*vertexCount], 0.0f),			// point 1
			glm::vec3(1.0f, heights[(gridX+1) + gridZ*vertexCount], 0.0f),	// point 2
			glm::vec3(0.0f, heights[gridX + (gridZ+1)*vertexCount], 1.0f),	// point 3
			glm::vec2(x, z)																									// position
		);

	} else { // x > (1 - z) - lower triangle
		height = Math::barryCentric(
			glm::vec3(1.0f, heights[(gridX+1) + (gridZ+1)*vertexCount], 1.0f),	// point 1
			glm::vec3(1.0f, heights[(gridX+1) + gridZ*vertexCount], 0.0f),			// point 2
			glm::vec3(0.0f, heights[gridX + (gridZ+1)*vertexCount], 1.0f),			// point 3
			glm::vec2(x, z)																											// position
		);
	}
	return height;
}