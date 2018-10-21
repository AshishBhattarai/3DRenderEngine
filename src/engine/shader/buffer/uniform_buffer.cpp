#include "uniform_buffer.hpp"

#include "utils/slogger.hpp"

GLuint UniformBuffer::usedBindingPoint = 0;

void UniformBuffer::createBuffer() {
	glGenBuffers(1, &UBO);
	// reserver space
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, total_size, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// set binding point
	bindingPoint = usedBindingPoint++;
	// glBindBufferRange(GL_UNIFORM_BUFFER, usedBindingPoint++, UBO, 0, size);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);
}

void UniformBuffer::setBufferData(const GLvoid* data, GLuint offset, GLuint size) {
	// check if buffer has enough space;
	if(size > total_size) {
		SLOG("Cannot set UBO, Not enough space. ", total_size, " ", size);
		return;
	}
	// set buffer
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::UniformBuffer(GLuint size):
		total_size(size)
{
	createBuffer();
}

// free buffer
UniformBuffer::~UniformBuffer()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDeleteBuffers(1, &UBO);
}
