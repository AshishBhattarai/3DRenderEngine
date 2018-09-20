#ifndef GENERAL_VS_UBO_HPP
#define GENERAL_VS_UBO_HPP

#include "uniform_buffer.hpp"

#include <glm/gtc/type_ptr.hpp>

// general vertex shader data

class GeneralVSUBO : public UniformBuffer {
private:																																				// Bytes
	static constexpr int PROJECTION_MAT_OFFSET = 0; 															// 0
	static constexpr int VIEW_MAT_OFFSET = SIZE_MATRIX4; 													// 64
	static constexpr int CAMERA_POS_OFFSET = 2 * SIZE_MATRIX4; 										// 128
	static constexpr int FOG_DENSITY_OFFSET = CAMERA_POS_OFFSET + SIZE_VEC3;			// 144
	static constexpr int FOG_GRADIENT_OFFSET = FOG_DENSITY_OFFSET + SIZE_BASIC; 	// 148

	// total size
	static constexpr int TOTAL_SIZE = FOG_GRADIENT_OFFSET + SIZE_BASIC; 					// 152 bytes

public:
	GeneralVSUBO():
		UniformBuffer(TOTAL_SIZE)
	{}

	// set projection matrix
	void setProjectionMatrix(const glm::mat4& mat) {
		setBufferData(glm::value_ptr(mat), PROJECTION_MAT_OFFSET, SIZE_MATRIX4);
	}

	// set view matrix
	void setViewMatrix(const glm::mat4& mat) {
		setBufferData(glm::value_ptr(mat), VIEW_MAT_OFFSET, SIZE_MATRIX4);
	}

	// set camera position
	void setCameraPos(const glm::vec3& pos) {
		setBufferData(glm::value_ptr(pos), CAMERA_POS_OFFSET, SIZE_VEC3);
	}

	void setFogDensity(float density) {
		setBufferData(&density, FOG_DENSITY_OFFSET, SIZE_BASIC);
	}

	void setFogGradient(float gradient) {
		setBufferData(&gradient, FOG_GRADIENT_OFFSET, SIZE_BASIC);
	}

	void setFogProperty(float density, float gradient) {
		setFogDensity(density);
		setFogGradient(gradient);
	}

};
#endif