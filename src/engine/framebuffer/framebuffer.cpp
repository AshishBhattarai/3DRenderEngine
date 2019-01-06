#include "framebuffer.hpp"

#include <glad/glad.h>

FrameBuffer::FrameBuffer(int width, int height):
	fbo(0),
	color_buffer(nullptr),
	depth_buffer(0),
	stencil_buffer(0),
	width(width),
	height(height),
	numColorAttach(0),
	color_type(NONE),
	depth_type(NONE),
	stencil_type(NONE),
	depth_stencil_type(NONE)
{
	glGenFramebuffers(1, &fbo);
}

FrameBuffer::~FrameBuffer() {
	//cleanup
	useDefault();
	clearColorAttachment();
	clearDepthAttachment();
	clearStencilAttachment();
	clearDepthStencilAttachment();

	// delete framebuffer
	glDeleteFramebuffers(1, &fbo);
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

void FrameBuffer::clearBuffer() {
	unsigned int clear = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	if(stencil_type != NONE || depth_stencil_type != NONE)
		clear |= GL_STENCIL_BUFFER_BIT;
	glClear(clear);
}

void FrameBuffer::cleanUp(AttachType type, unsigned int* buffer, int num) {
	if(type == TBUFFER) {
		glDeleteTextures(num, buffer);
	} else if(type == RBUFFER) {
		glDeleteRenderbuffers(num, buffer);
	}
	type = NONE;
}

void FrameBuffer::setColorAttachment(AttachType type, unsigned int numColorAttach) {
	color_type = type;
	this->numColorAttach = numColorAttach;
	color_buffer = new unsigned int[numColorAttach];

	if(type == TBUFFER) {
		glGenTextures(numColorAttach, color_buffer);
		for(unsigned int i = 0; i < numColorAttach; ++i) {
			glBindTexture(GL_TEXTURE_2D, color_buffer[i]);
			// create an empty texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_HALF_FLOAT, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			// attach texture to fbo
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, color_buffer[i], 0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	} else if(type == RBUFFER) {
		glGenRenderbuffers(numColorAttach, color_buffer);
		for(unsigned int i = 0; i < numColorAttach; ++i) {
			// create rbo
			glBindRenderbuffer(GL_RENDERBUFFER, color_buffer[i]);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_SRGB, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_RENDERBUFFER, color_buffer[i]);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

void FrameBuffer::setDepthAttachment(AttachType type) {
	depth_type = type;
	if(type == TBUFFER) {
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
		glBindTexture(GL_TEXTURE_2D, 0);
	} else if(type == RBUFFER) {
		glGenRenderbuffers(1, &depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

void FrameBuffer::setStencilAttachment(AttachType type) {
	stencil_type = type;
	if(type == TBUFFER) {
		glGenTextures(1, &stencil_buffer);
		glBindTexture(GL_TEXTURE_2D, stencil_buffer);

		// Stencil ONLY texture available only in opengl 4.4+
		glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, width, height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencil_buffer, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	} else if(type == RBUFFER) {
		glGenRenderbuffers(1, &stencil_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, stencil_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencil_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

void FrameBuffer::setDepthStencilAttachment(AttachType type) {
	depth_stencil_type = type;
	if(type == TBUFFER) {
		glGenTextures(1, &depth_buffer);
		glBindTexture(GL_TEXTURE_2D, depth_buffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_buffer, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	} else if(type == RBUFFER) {
		glGenRenderbuffers(1, &depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

bool FrameBuffer::isComplete() {
	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}