#ifndef TYPE_CONVERSION_HPP
#define TYPE_CONVERSION_HPP

#include <glm/glm.hpp>
#include <bullet/LinearMath/btVector3.h>

namespace VEC3 {

	inline btVector3 glmToBt(const glm::vec3& vec) {
		return btVector3(vec.x, vec.y, vec.z);
	}

	inline glm::vec3 btToGlm(const btVector3& vec) {
		return glm::vec3(vec.x(), vec.y(), vec.z());
	}

};

#endif