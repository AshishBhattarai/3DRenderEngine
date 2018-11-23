#ifndef AABB_MESH_HPP
#define AABB_MEHS_HPP

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "shape/aabb.hpp"

class AABBMesh {
private:
	GLuint vao;
	GLuint vbo;
	GLuint ibo;

public:
	AABBMesh(bool static_aabb = true);

	~AABBMesh();

	void updateData(const AABB& aabb);

	GLuint getVAO();

};

#endif