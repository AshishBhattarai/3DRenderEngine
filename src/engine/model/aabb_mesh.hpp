#ifndef AABB_MESH_HPP
#define AABB_MEHS_HPP

#include <glm/glm.hpp>
#include <glad/glad.h>

class AABBMesh {
private:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;

public:
	AABBMesh(bool static_aabb = true);

	~AABBMesh();

	void updateData(glm::vec3 minBB, glm::vec3 maxBB);

	GLuint getVAO();

};

#endif