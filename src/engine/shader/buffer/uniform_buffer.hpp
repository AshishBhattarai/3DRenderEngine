#ifndef UNIFORM_BUFFER_HPP
#define UNIFORM_BUFFER_HPP

#include <glad/glad.h>

// uniform buffer for common uniforms among shaders

class UniformBuffer {
public:
	// NOTE: std140 offset should be multiple of 16
	// types base alignment(ie: the space(size in bytes) taken by the variable)
	constexpr static int SIZE_BASIC 	= 	4; // int, float, bool
	constexpr static int SIZE_VEC2 		= 	8;
	constexpr static int SIZE_VEC3 		=	 16;
	constexpr static int SIZE_VEC4 		=	 16;
	constexpr static int SIZE_MATRIX4 =  4*SIZE_VEC4;
	constexpr static int SIZE_MATRIX3 =	 3*SIZE_VEC4;

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

	const GLuint getUBO() const {
		return UBO;
	}

	const GLuint getBindingPoint() const {
		return bindingPoint;
	}

	const GLuint getTotalSize() const {
		return total_size;
	}
};

#endif