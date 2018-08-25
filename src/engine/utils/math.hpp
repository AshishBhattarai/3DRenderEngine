#ifndef MATH_HPP
#define MATH_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Math {

	/** Barycentric Interpolation
		Takes three points(vertices) of a triangle and a position(inside the triangle)
			and returns its corresponding height.
	**/
	inline constexpr float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
		// convert to barycentric coords l1, l2, l3 for a point pos in the triangle
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;
		// interpolate height
		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}
};

#endif