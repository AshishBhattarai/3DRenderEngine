#include "mesh.hpp"

void Mesh::setupMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &v_VBO);
	glGenBuffers(1, &EBO);

	// bind
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, v_VBO);
	// load vertex data - &verteces[0], sending pointer to first element of vector(sequential memory)
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	// set vertex attribs
	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
	glEnableVertexAttribArray(0);
	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	attributesCount = 2; // no. of attributes set
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices,
		std::string name) :
		indicesCount(indices.size()), //NOTE: imp don't miss this in another MESH-REDO
		name(name)
{
		setupMesh(vertices, indices);
}

// destroy
Mesh::~Mesh() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &v_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}
