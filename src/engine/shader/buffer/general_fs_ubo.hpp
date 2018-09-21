#ifndef GENERAL_FS_UBO_HPP
#define GENERAL_FS_UBO_HPP

#include "uniform_buffer.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "entities/light.hpp"

// TODO: multiple suns

class GeneralFSUBO : public UniformBuffer {
private:
	// offsets																										// offset bytes
	static constexpr int FOG_COLOR_OFFSET = 0;										//		0
	// directional light	-	dir + colors
	static constexpr int DIR_LIGHT_OFFSET	=	SIZE_VEC3;					  //	  64
	// total
	static constexpr int TOTAL_SIZE = 5*SIZE_VEC3;

public:
	GeneralFSUBO():
		UniformBuffer(TOTAL_SIZE)
	{}

	// set sky color
	void setFogColor(const glm::vec3& color) {
		setBufferData(glm::value_ptr(color), FOG_COLOR_OFFSET, SIZE_VEC3);
	}

	// set sun
	void setSun(const Light& light) {
		// set direction
		setBufferData(glm::value_ptr(light.getPosition()), DIR_LIGHT_OFFSET, SIZE_VEC3);
		// set 3 colors
		setBufferData(glm::value_ptr(light.getAmbient()), DIR_LIGHT_OFFSET	+	SIZE_VEC3, SIZE_VEC3);
		setBufferData(glm::value_ptr(light.getDiffuse()), DIR_LIGHT_OFFSET	+	2*SIZE_VEC3, SIZE_VEC3);
		setBufferData(glm::value_ptr(light.getSpecular()), DIR_LIGHT_OFFSET	+	3*SIZE_VEC3, SIZE_VEC3);
	}
};

#endif