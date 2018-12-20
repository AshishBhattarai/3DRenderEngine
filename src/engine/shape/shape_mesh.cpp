#include "shape_mesh.hpp"

#include <glad/glad.h>

ShapeMesh::ShapeMesh(float* data, int dim, int numVertex, bool normal, bool texCoords) :
	VAO(0),
	VBO(0),
	EBO(0),
	count(numVertex)
{
	int stride = dim; // position is must
	int tex_offset = 0;

	if(normal) { // optional normal
		stride += dim;
	}
	if(texCoords) { // option texture coords
		stride += 2;
		if(normal) {
			tex_offset = 2*dim;
		} else {
			tex_offset = dim;
		}
	}
	stride *= sizeof(float);

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);

	// size = stride * numVerte * sizeof(float)
	glBufferData(GL_ARRAY_BUFFER, stride * numVertex, data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, dim, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);
	if(normal) {
		glVertexAttribPointer(1, dim, GL_FLOAT, GL_FALSE, stride, (void*)(dim*sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	if(texCoords) {
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(tex_offset*sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ShapeMesh::~ShapeMesh() {
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ShapeMesh::setIndices(unsigned int* data, int numIndices) {
	count = numIndices;

	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(unsigned int), data, GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ShapeMesh::draw() {
	glBindVertexArray(VAO);
	if(EBO)
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, count);
	glBindVertexArray(0);
}