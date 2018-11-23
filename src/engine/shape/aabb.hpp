#ifndef AABB_HPP
#define AABB_HPP

#include <glm/glm.hpp>
#include <array>

class AABB {
	// comparison operators
	friend bool operator==(const AABB& lhs, const AABB& rhs);
	friend bool operator!=(const AABB& lhs, const AABB& rhs);

private:
	// local coord aabb at - 0.0f, 0.0f, 0.0f
	glm::vec3 local_minBB;
	glm::vec3 local_maxBB;

	// world coord aabb at - position x, y, z
	glm::vec3 world_minBB;
	glm::vec3 world_maxBB;

	void rotate(const glm::vec3& rotation, glm::vec3& minBB, glm::vec3& maxBB);

public:
	AABB(const glm::vec3& local_minBB, const glm::vec3& local_maxBB);

	/**
	 * onLocal
	 * 	-> true - apply transformation on local aabb coords.
	 * 	-> false - apply transformation on world aabb coords.
	 **/
	void transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale, bool onLocal = true);

	// plane xyz - normal, w - offset
	bool planeCollision(const glm::vec4& plane);

	// check if aabb lies inside-intersects with given frustum
	bool inFrustum(const std::array<glm::vec4, 6>& frustum);

	// setters and getters are for world coords only
	void setMinBB(const glm::vec3& minBB) {
		world_minBB = minBB;
	}

	void setMaxBB(const glm::vec3& maxBB) {
		world_maxBB = maxBB;
	}

	void setMinMaxBB(const glm::vec3& minBB, const glm::vec3& maxBB) {
		world_minBB = minBB;
		world_maxBB = maxBB;
	}

	glm::vec3 getMinBB() const {
		return world_minBB;
	}

	glm::vec3 getMaxBB() const {
		return world_maxBB;
	}
};

inline bool operator==(const AABB& lhs, const AABB& rhs) {
	return (lhs.world_minBB == rhs.world_minBB && lhs.world_maxBB == rhs.world_maxBB);
}

inline bool operator!=(const AABB& lhs, const AABB& rhs) {
	return !operator==(lhs, rhs);
}
#endif