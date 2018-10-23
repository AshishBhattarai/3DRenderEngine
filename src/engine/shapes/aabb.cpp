#include "aabb.hpp"
#include "utils/slogger.hpp"

#include <glm/gtc/matrix_transform.hpp>

AABB::AABB(const glm::vec3& local_minBB, const glm::vec3& local_maxBB):
	local_minBB(local_minBB),
	local_maxBB(local_maxBB),
	world_minBB(local_minBB),
	world_maxBB(local_maxBB)
{

}

void AABB::rotate(const glm::vec3& rotation, glm::vec3& minBB, glm::vec3& maxBB) {
	if(rotation == glm::vec3(0.0f)) return;

	glm::mat4 rmat(1.0f);
	// y first to match bullet
	rmat = glm::rotate(rmat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rmat = glm::rotate(rmat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rmat = glm::rotate(rmat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 halfsize = (maxBB - minBB) * 0.5f;
	glm::vec3 offset = (maxBB + minBB) * 0.5f;
	glm::vec3 center = glm::vec3(rmat * glm::vec4((offset), 0.0f));

	glm::vec3 rot;
	rot.x = fabs(rmat[0].x * halfsize.x) + fabs(rmat[1].x * halfsize.y) + fabs(rmat[2].x * halfsize.z);
	rot.y = fabs(rmat[0].y * halfsize.x) + fabs(rmat[1].y * halfsize.y) + fabs(rmat[2].y * halfsize.z);
	rot.z = fabs(rmat[0].z * halfsize.x) + fabs(rmat[1].z * halfsize.y) + fabs(rmat[2].z * halfsize.z);

	minBB = center - rot;
	maxBB = center + rot;
}

void AABB::transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale,  bool onLocal) {
	glm::mat4 mat(1.0f);
	glm::vec3 minBB = local_minBB;
	glm::vec3 maxBB = local_maxBB;
	if(!onLocal) { // apply to world coords
		minBB = world_minBB;
		maxBB = world_maxBB;
	}

	// scale
	mat = glm::scale(mat, scale);
	minBB = glm::vec3(mat * glm::vec4(minBB, 1.0f));
	maxBB = glm::vec3(mat * glm::vec4(maxBB, 1.0f));

	// rotate
	rotate(rotation, minBB, maxBB);

	// translate
	mat = glm::translate(glm::mat4(1.0f), position);
	minBB = glm::vec3(mat * glm::vec4(minBB, 1.0f));
	maxBB = glm::vec3(mat * glm::vec4(maxBB, 1.0f));

	world_minBB = minBB;
	world_maxBB = maxBB;
}

bool AABB::planeCollision(const glm::vec4& plane) {
	glm::vec3 halfsize = (world_maxBB - world_minBB) * 0.5f;
	glm::vec3 center = world_minBB + halfsize; // center = min + halfsize or max - halfsize

	// radius = dot product between plane normal(xyz) and halfsize - using fabs vals
	float radius = fabs(plane.x*halfsize.x) + fabs(plane.y*halfsize.y) + fabs(plane.z*halfsize.z);

	// distance of box center from plane
	float dist = glm::dot(glm::vec3(plane), center) - plane.w;

	// if dist within [-r, +r] intersection/collision.
	return abs(dist) <= radius;
}

bool AABB::inFrustum(const std::array<glm::vec4, 6>& frustum) {
	glm::vec3 maxBB(0.0f);

	// loop through all six planes
	for(auto& plane : frustum) {
		// set maxBB - checking if plane is on positive or negative quadrant.
		maxBB.x = (plane.x > 0.0f)? world_maxBB.x : world_minBB.x;
		maxBB.y = (plane.y > 0.0f)? world_maxBB.y : world_minBB.y;
		maxBB.z = (plane.z > 0.0f)? world_maxBB.z : world_minBB.z;

		/**
		 * Dot product between plane normal and maximum aabb point(maxBB).
		 * if < 0.0f ie maxBB lies behind/outside the plane,
		 * 							Hence the AABB lies outside the plane.
		 */
		if(glm::dot(glm::vec3(plane), maxBB) + plane.w < 0.0f)
			return false; // outside
	}
	// if not outside any of the six frustum planes
	return true; // inside
}