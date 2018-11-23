#include "framebuffer.hpp"

#include <glad/glad.h>

FrameBuffer::FrameBuffer(int width, int height, unsigned int numColorAttach):
	width(width),
	height(height),
	numColorAttach(numColorAttach),
	color_type(NONE),
	depth_type(NONE),
	stencil_type(NONE),
	depth_stencil_type(NONE)
{
	if(numColorAttach)
		color_buffer = new unsigned int[numColorAttach];
	glGenFramebuffers(1, &fbo);
}

FrameBuffer::~FrameBuffer() {
	//cleanup
	useDefault();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// delete color attachment
	cleanUp(color_type, color_buffer, numColorAttach);

	// delete depth or depth_stencil attachment
	cleanUp(depth_type, &depth_buffer, 1);

	// delete stencil attachment
	cleanUp(stencil_type, &stencil_buffer, 1);

	// delete depth_stencil attachment
	cleanUp(depth_stencil_type, &depth_buffer, 1);

	// delete framebuffer
	glDeleteFramebuffers(1, &fbo);
	delete[] color_buffer;
}

void FrameBuffer::use(UseType type) {
	if(type == NORMAL) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	} else if(type == READ) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	} else if(type == DRAW) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	}
}

void FrameBuffer::useDefault() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::cleanUp(AttachType type, unsigned int* buffer, int num) {
	if(type == TEXTURE) {
		glDeleteTextures(num, buffer);
	} else if(type == RBUFFER) {
		glDeleteRenderbuffers(num, buffer);
	}
}

void FrameBuffer::setColorAttachment(AttachType type) {
	color_type = type;
	if(type == TEXTURE) {
		glGenTextures(numColorAttach, color_buffer);
		for(unsigned int i = 0; i < numColorAttach; ++i) {
			glBindTexture(GL_TEXTURE_2D, color_buffer[i]);
			// create an empty texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// attach texture to fbo
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, color_buffer[i], 0);
		}
	} else if(type == RBUFFER) {
		glGenRenderbuffers(numColorAttach, color_buffer);
		for(unsigned int i = 0; i < numColorAttach; ++i) {
			// create rbo
			glBindRenderbuffer(GL_RENDERBUFFER, color_buffer[i]);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_RENDERBUFFER, color_buffer[i]);
		}
	}
}

void FrameBuffer::setDepthAttachment(AttachType type) {
	depth_type = type;
	if(type == TEXTURE) {
		glGenTextures(1, &depth_buffer);
		glBindTexture(GL_TEXTURE_2D, depth_buffer);

		// allocate depth texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		// parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_buffer, 0);
	} else if(type == RBUFFER) {
		glGenRenderbuffers(1, &depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	}
}

void FrameBuffer::setStencilAttachment(AttachType type) {
	stencil_type = type;
	if(type == TEXTURE) {
		glGenTextures(1, &stencil_buffer);
		glBindTexture(GL_TEXTURE_2D, stencil_buffer);

		// Stencil ONLY texture available only in opengl 4.4+
		glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencil_buffer, 0);
	} else if(type == RBUFFER) {
		glGenRenderbuffers(1, &stencil_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, stencil_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil_buffer);
	}
}

void FrameBuffer::setDepthStencilAttachment(AttachType type) {
	depth_stencil_type = type;
	if(type == TEXTURE) {
		glGenTextures(1, &depth_buffer);
		glBindTexture(GL_TEXTURE_2D, depth_buffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_buffer, 0);
	} else if(type == RBUFFER) {
		glGenRenderbuffers(1, &depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
	}
}

bool FrameBuffer::isComplete() {
	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}