#ifndef MATH_HPP
#define MATH_HPP

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Math {

	/** Barycentric Interpolation
		Takes three points(vertices) of a triangle and a position(inside the triangle)
			and returns its corresponding height.
	**/
	inline float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
		// convert to barycentric coords l1, l2, l3 for a point pos in the triangle
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		// interpolate height
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

	inline std::array<glm::vec4, 6> extractViewFrustum(const glm::mat4& proj, const glm::mat4& view) {
		glm::mat4 mv = proj * view;

		glm::vec4 top(mv[0][3] - mv[0][1], mv[1][3] - mv[1][1], mv[2][3] - mv[2][1], mv[3][3] - mv[3][1]);
		glm::vec4 bottom(mv[0][3] + mv[0][1], mv[1][3] + mv[1][1], mv[2][3] + mv[2][1], mv[3][3] + mv[3][1]);

		glm::vec4 right(mv[0][3] - mv[0][0], mv[1][3] - mv[1][0], mv[2][3] - mv[2][0], mv[3][3] - mv[3][0]);
		glm::vec4 left(mv[0][3] + mv[0][0], mv[1][3] + mv[1][0], mv[2][3] + mv[2][0], mv[3][3] + mv[3][0]);

		glm::vec4 near(mv[0][3] + mv[0][2], mv[1][3] + mv[1][2], mv[2][3] + mv[2][2], mv[3][3] + mv[3][2]);
		glm::vec4 far(mv[0][3] - mv[0][2], mv[1][3] - mv[1][2], mv[2][3] - mv[2][2], mv[3][3] - mv[3][2]);

		return std::array<glm::vec4, 6>({right, left, top, bottom, near, far});
	}

	inline void rotateBB(glm::vec3& minBB, glm::vec3& maxBB, const glm::vec3& rotation) {
		if(rotation == glm::vec3(0.0f)) return;

		glm::mat4 rmat(1.0f);
		rmat = glm::rotate(rmat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		rmat = glm::rotate(rmat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
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
};

#endif