#ifndef GENERAL_FS_UBO_HPP
#define GENERAL_FS_UBO_HPP

#include "uniform_buffer.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "entities/light.hpp"
#include "entities/point_light.hpp"
#include "../shader_config.hpp"

// TODO: multiple suns

class GeneralFSUBO : public UniformBuffer {
private:
	// sizes
	static constexpr int DIR_LIGHT_SIZE = 3*SIZE_VEC3;	// 48
	static constexpr int POINT_LIGHT_SIZE = 3*SIZE_VEC3 + SIZE_VEC4; // 64

	// offsets																																				// offset
	static constexpr int FOG_COLOR_OFFSET = 0;																				//		0
	// scene ambient factor
	static constexpr int AMBIENT_OFFSET = SIZE_VEC3; 																	//		16
	// directional light	-	dir + colors
	static constexpr int DIR_LIGHT_OFFSET	=	AMBIENT_OFFSET + 4*SIZE_BASIC;						//	  32
	// num of active point lights
	static constexpr int NUM_POINT_LIGHT_OFFSET = DIR_LIGHT_OFFSET + DIR_LIGHT_SIZE;	//		80
	// point lights
	static constexpr int POINT_LIGHT_OFFSET = NUM_POINT_LIGHT_OFFSET + 4*SIZE_BASIC;	//		96
	// total
	static constexpr int TOTAL_SIZE = POINT_LIGHT_OFFSET + ShaderConfig::MAX_POINT_LIGHTS * POINT_LIGHT_SIZE; // 736

public:
	GeneralFSUBO():
		UniformBuffer(TOTAL_SIZE)
	{}

	// set sky color
	void setFogColor(const glm::vec3& color) {
		setBufferData(glm::value_ptr(color), FOG_COLOR_OFFSET, SIZE_VEC3);
	}

	void setAmbient(float ambient) {
		setBufferData(&ambient, AMBIENT_OFFSET, SIZE_BASIC);
	}

	// set sun
	void setSun(const Light& light) {
		// set direction
		setBufferData(glm::value_ptr(light.getPosition()), DIR_LIGHT_OFFSET, SIZE_VEC3);
		// set 2 colors
		setBufferData(glm::value_ptr(light.getDiffuse()), DIR_LIGHT_OFFSET	+	SIZE_VEC3, SIZE_VEC3);
		setBufferData(glm::value_ptr(light.getSpecular()), DIR_LIGHT_OFFSET	+	2*SIZE_VEC3, SIZE_VEC3);
	}

	void setNumPointLight(int num) {
		setBufferData(&num, NUM_POINT_LIGHT_OFFSET, SIZE_BASIC);
	}

	void setPointLight(const PointLight& light, unsigned int index) {
		unsigned int offset = POINT_LIGHT_OFFSET + index*POINT_LIGHT_SIZE;

		setBufferData(glm::value_ptr(light.getPosition()), offset, SIZE_VEC3);
		setBufferData(glm::value_ptr(light.getDiffuse()), offset + SIZE_VEC3, SIZE_VEC3);
		setBufferData(glm::value_ptr(light.getSpecular()), offset + 2*SIZE_VEC3, SIZE_VEC3);

		offset += 3*SIZE_VEC3;
		setBufferData(glm::value_ptr(light.getAttenuation()), offset, SIZE_VEC4);
	}
};

#endif