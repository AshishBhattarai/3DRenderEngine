#include "aabb_mesh.hpp"

AABBMesh::AABBMesh(bool static_aabb) {
	GLenum draw_type;
	GLuint indexData[] = {
		0, 1, 3, 3, 1, 2,	4, 5, 7, 7, 5, 6, 8, 9, 11, 11, 9, 10, 12, 13,
		15, 15,13, 14, 16, 17, 19, 19, 17, 18, 20, 21, 23, 23, 21, 22
   };

	// for non static entites aabb is updated almost each frame
	if(static_aabb)
		draw_type = GL_STATIC_DRAW;
	else
		draw_type = GL_DYNAMIC_DRAW;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6*4*3*sizeof(float), NULL, draw_type);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*2*3*sizeof(GLuint), indexData, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
}

AABBMesh::~AABBMesh() {
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void AABBMesh::updateData(const AABB& aabb) {
	glm::vec3 vertices[24];
	glm::vec3 minBB = aabb.getMinBB();
	glm::vec3 maxBB = aabb.getMaxBB();

	// back
	vertices[0] = glm::vec3(minBB.x, maxBB.y, maxBB.z);
	vertices[1] = glm::vec3(minBB.x, minBB.y, maxBB.z);
	vertices[2] = glm::vec3(maxBB.x, minBB.y, maxBB.z);
	vertices[3] = maxBB;

	// front
	vertices[4] = glm::vec3(minBB.x, maxBB.y, minBB.z);
	vertices[5] = glm::vec3(maxBB.x, maxBB.y, minBB.z);
	vertices[6] = glm::vec3(maxBB.x, minBB.y, minBB.z);
	vertices[7] = minBB;

	// right
	vertices[8] = maxBB;
	vertices[9] = glm::vec3(maxBB.x, minBB.y, maxBB.z);
	vertices[10] = glm::vec3(maxBB.x, minBB.y, minBB.z);
	vertices[11] = glm::vec3(maxBB.x, maxBB.y, minBB.z);

	// left
	vertices[12] = glm::vec3(minBB.x, maxBB.y, maxBB.z);
	vertices[13] = glm::vec3(minBB.x, maxBB.y, minBB.z);
	vertices[14] = minBB;
	vertices[15] = glm::vec3(minBB.x, minBB.y, maxBB.z);

	// top
	vertices[16] = glm::vec3(minBB.x, maxBB.y, minBB.z);
	vertices[17] = glm::vec3(minBB.x, maxBB.y, maxBB.z);
	vertices[18] = maxBB;
	vertices[19] = glm::vec3(maxBB.x, maxBB.y, minBB.z);

	// bottom
	vertices[20] = minBB;
	vertices[21] = glm::vec3(maxBB.x, minBB.y, minBB.z);
	vertices[22] = glm::vec3(maxBB.x, minBB.y, maxBB.z);
	vertices[23] = glm::vec3(minBB.x, minBB.y, maxBB.z);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 6*4*3*sizeof(float), vertices);
}

GLuint AABBMesh::getVAO() {
	return vao;
}
