#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include <glad/glad.h>

// uniform buffer for common uniforms among shaders

class UniformBuffer {
public:
	// NOTE: avoid using vec3 on uniform block, use vec4 and swizzle it (vec4.xyz).
	// types base alignment(ie: the space(size in bytes) taken by the variable)
	constexpr static int SIZE_BASIC 	= 	4; // int, float, bool (N) - N = sizeof(float)
	constexpr static int SIZE_VEC2 		= 	8; // 2*N
	constexpr static int SIZE_VEC3 		=	 16; // 4*N
	constexpr static int SIZE_VEC4 		=	 16; // 4*N
	constexpr static int SIZE_MATRIX4 =  4*SIZE_VEC4; // 16*N
	constexpr static int SIZE_MATRIX3 =	 3*SIZE_VEC4; // 12*N

private:
	GLuint UBO;
	GLuint total_size;
	GLuint bindingPoint; // binding pointer for this buffer
	static GLuint usedBindingPoint; // counts the binding points used buy UniformBuffer's

	// create a buffer of size =
	void createBuffer();

public:
	UniformBuffer(GLuint size);

	// free buffer
	~UniformBuffer();

	// set buffer subdata
	void setBufferData(const GLvoid* data, GLuint offset, GLuint size);

	GLuint getUBO() const {
		return UBO;
	}

	GLuint getBindingPoint() const {
		return bindingPoint;
	}

	GLuint getTotalSize() const {
		return total_size;
	}
};

#endif