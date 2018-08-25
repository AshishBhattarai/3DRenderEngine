#ifndef GENERAL_VS_UBO_HPP
#define GENERAL_VS_UBO_HPP

#include "uniform_buffer.hpp"

#include <glm/gtc/type_ptr.hpp>

// general vertex shader data

class GeneralVSUBO : public UniformBuffer {
private:
	// offset of buffers (must be multiple of 16)													//Bytes
	static constexpr int PROJECTION_MAT_OFFSET = 0; 											// 0
	static constexpr int VIEW_MAT_OFFSET = SIZE_MATRIX4; 									// 64
	static constexpr int CAMERA_POS_OFFSET = 2 * SIZE_MATRIX4; 						// 128
	// TODO:Global lights too maybe? or create new class

	// total size
	static constexpr int TOTAL_SIZE = CAMERA_POS_OFFSET + SIZE_VEC3; // 144 bytes

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
};
#endif