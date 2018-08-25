#include "textured_mesh.hpp"

// load texture coords
void TexturedMesh::setupTextureCoords(std::vector<glm::vec2>& texCoords) {
	glGenBuffers(1, &t_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, t_VBO);
	glBindVertexArray(VAO);
	// load texture data
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2),
			&texCoords[0], GL_STATIC_DRAW);
	// set vertex attribute
	glVertexAttribPointer(attributesCount, 2, GL_FLOAT, GL_FALSE,
			sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(attributesCount);
	// unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDisableVertexAttribArray(attributesCount++);
}

// no texture (only coords)
TexturedMesh::TexturedMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices,
		std::vector<glm::vec2>& texCoords, std::string name) :
		Mesh(vertices, indices, name),
		hasTransparent(false)
{
	setupTextureCoords(texCoords);
}

// single diffuse texture
TexturedMesh::TexturedMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices,
		std::vector<glm::vec2>& texCoords, std::shared_ptr<Texture> tex_diffuse, std::string name) :
		TexturedMesh(vertices, indices, texCoords, name) // delegate
{
	textures[tex_diffuse->getType()] = tex_diffuse;
}

// multiple(map) textures
TexturedMesh::TexturedMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices,
		std::vector<glm::vec2>& texCoords, Texture::Map&& textures,
		float shine, float specular_factor, std::string name) :
		Mesh(vertices, indices, name),
		textures(textures),
		specular_shine(shine),
		specular_factor(specular_factor),
		hasTransparent(false)
{
	setupTextureCoords(texCoords);
}

TexturedMesh::~TexturedMesh() {
	// free buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &t_VBO);
}