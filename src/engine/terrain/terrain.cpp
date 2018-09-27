#include "terrain.hpp"

#include "utils/slogger.hpp"
#include "utils/math.hpp"

float Terrain::calcHeightAt(u_int x, u_int z, Image& heightMap) {
	if (x >= (u_int)heightMap.getWidth() || z >= (u_int)heightMap.getHeight())
		return 0.0f;

	// get pixel color (bottom-right of heightmap as (0, 0))
  float height = heightMap.getRGB(heightMap.getWidth() - 1 - x, heightMap.getHeight() - 1 - z);
	// calc height
	height -= (MAX_PIXEL_COLOR/2.0f); // between -MAX_PIXEL_COLOR/2 and MAX_PIXEL_COLOR/2
	height /= (MAX_PIXEL_COLOR/2.0f);	// between -1 & 1

	height *= maxHeight; // between -MAX_HEIGHT & MAX_HEIGHT
  return height;
}

// calculate normal baised on heights of surrounding(neighbour) vertices, (finite difference method)
glm::vec3 Terrain::calcNormalAt(u_int x, u_int z, Image& heightMap) {
	float heightL = calcHeightAt(x-1, z, heightMap); // left
	float heightR = calcHeightAt(x+1, z, heightMap); // right
	float heightU = calcHeightAt(x, z+1, heightMap); // up
	float heightD = calcHeightAt(x, z-1, heightMap); // down
	// calculate normal
	glm::vec3 normal(heightL-heightR, 2.0f, heightD-heightU);
	return glm::normalize(normal);
}

// generate vertex data
void Terrain::generateVertices(
	std::vector<TexturedMesh::Vertex>& vertices, std::vector<glm::vec2>& texCoords)
{
	for(u_int z = 0; z < vertexCount; ++z) { // z - h
		for(u_int x = 0; x < vertexCount; ++x) { // x - w
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
	for(u_int z = 0; z < vertexCount-1; ++z) { // height
		for(u_int x = 0; x < vertexCount; ++x) { // width
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
void Terrain::generateHeightWMap(std::vector<TexturedMesh::Vertex>& vertices, Image& image)
{
	u_int i, j;
	for(u_int z = 0; z < vertexCount; ++z) {
		for(u_int x = 0; x < vertexCount; ++x) {
			i = x + z*(vertexCount);
			j = x + z*(vertexCount+1); // +1 to comply with terrian_collision_shape
			// calc height
			heights[j] = calcHeightAt(x, z, image);
			//vertex height
			vertices[i].position.y = heights[j];
			// normals
			vertices[i].normal = calcNormalAt(x, z, image);
		}
	}
}

// takes Terrain size, vertex count, and repeate factor for Terrain texture
std::unique_ptr<TexturedMesh> Terrain::generateTerrain(
	std::function<void(std::vector<TexturedMesh::Vertex>&)> generateHeight)
{
	u_int total_vertices = vertexCount * vertexCount;
	// vectors to store mesh data
	std::vector<Mesh::Vertex> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<u_int> indices;

	// reserve space
	vertices.reserve(total_vertices);
	texCoords.reserve(total_vertices);
	indices.reserve(2*vertexCount*vertexCount);

	// vertices
	generateVertices(vertices, texCoords);
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
Terrain::Terrain(u_int gridX, u_int gridZ, int size, u_int vertexCount, float maxHeight, Mode mode) :
	mode(mode),
	vertexCount(vertexCount),
	size(size),
	posX(gridX*size),
	posZ(gridZ*size),
	gridSize((float)size/(vertexCount - 1)), // Total grids per row&col
	maxHeight(maxHeight),
	heights((vertexCount+1)*(vertexCount+1), 0.0f) // reserve
{
	// incase the the vertex count is 0 - to avoid crash
	if(!vertexCount) this->vertexCount = 1;
}

// flat Terrain
Terrain::Terrain(u_int gridX, u_int gridZ, int size, u_int vertexCount, Texture::Map& textures) :
	Terrain(gridX, gridZ, size, vertexCount, 0.0f, FLAT)
{
	mesh = generateTerrain();
	mesh->setTextureMap(textures);
}

// height map Terrain
Terrain::Terrain(u_int gridX, u_int gridZ, int size, float maxHeight, Image& heightMap,
	Texture::Map& textures) :
	Terrain(gridX, gridZ, size, heightMap.getWidth(), maxHeight, HEIGHT_MAP)
{
	// generateTerrain with heightMap
	mesh = generateTerrain(
		[this, &heightMap](std::vector<TexturedMesh::Vertex>& vertices)
	{
		generateHeightWMap(vertices, heightMap);
	});

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
	u_int vc = vertexCount + 1;

	// get the tringle on which posX & posZ lie
	if(x <= (1 - z)) { // upper triangle
		// calculate height at (x, z) by interpolating know heights
		height = Math::barryCentric(
			glm::vec3(0.0f, heights[gridX + gridZ*vc], 0.0f),			// point 1
			glm::vec3(1.0f, heights[(gridX+1) + gridZ*vc], 0.0f),	// point 2
			glm::vec3(0.0f, heights[gridX + (gridZ+1)*vc], 1.0f),	// point 3
			glm::vec2(x, z)																									// position
		);

	} else { // x > (1 - z) - lower triangle
		height = Math::barryCentric(
			glm::vec3(1.0f, heights[(gridX+1) + (gridZ+1)*vc], 1.0f),	// point 1
			glm::vec3(1.0f, heights[(gridX+1) + gridZ*vc], 0.0f),			// point 2
			glm::vec3(0.0f, heights[gridX + (gridZ+1)*vc], 1.0f),			// point 3
			glm::vec2(x, z)																											// position
		);
	}
	return height;
}

void Terrain::getTransMatrix(glm::mat4& trans) {
	trans = glm::translate(glm::mat4(1.0f), glm::vec3(posX, 0.0f, posZ));
}