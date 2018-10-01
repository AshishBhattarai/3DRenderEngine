#ifndef AABB_MESH_HPP
#define AABB_MEHS_HPP

#include <glm/glm.hpp>
#include <glad/glad.h>

class AABBMesh {
private:
	u_int vao;
	u_int vbo;
	u_int ibo;

public:
	AABBMesh(bool static_aabb = true);

	~AABBMesh();

	void updateData(glm::vec3 minBB, glm::vec3 maxBB);

	u_int getVAO();

};

#endif