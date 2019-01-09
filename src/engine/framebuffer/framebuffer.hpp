#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <functional>

// This class represents opengl fbo
class FrameBuffer {
public:
	enum AttachType {
		NONE,
		TBUFFER, // TEXTURE BUFFER
		RBUFFER // RENDER BUFFER
	};

	enum UseType {
		NORMAL,
		READ,
		DRAW
	};

private:
	unsigned int fbo;
	unsigned int* color_buffer; // dynamic array for multiple color attachments/buffer
	unsigned int depth_buffer; // depth_stencil incase of GL_DEPTH24_STENCIL8
	unsigned int stencil_buffer;

	// framebuffer size
	int width;
	int height;
	unsigned int numColorAttach; // number of color attachments

	AttachType color_type;
	AttachType depth_type;
	AttachType stencil_type;
	AttachType depth_stencil_type;

	// function cleanup allocated buffers
	void cleanUp(AttachType type, unsigned int* buffer, int num);

public:
	FrameBuffer(int width, int height);
	~FrameBuffer();

	// set framebuffer attachments
	void setColorAttachment(AttachType type, unsigned int numColorAttach = 1);
	void setDepthAttachment(AttachType type);
	void setStencilAttachment(AttachType type);
	void setDepthStencilAttachment(AttachType type);

	bool isComplete();
	void use(UseType type = NORMAL);
	void clearBuffer();
	static void useDefault();

	void loadViewport();
	static void loadDefaultViewport();

	// clean ups
	std::function<void()> clearColorAttachment = [this]	{
		cleanUp(color_type, color_buffer, numColorAttach);
		if(color_buffer) color_buffer = (delete[] color_buffer, nullptr);
	};
	std::function<void()> clearDepthAttachment = [this]{cleanUp(depth_type, &depth_buffer, 1);};
	std::function<void()> clearStencilAttachment = [this]{cleanUp(stencil_type, &stencil_buffer, 1);};
	std::function<void()> clearDepthStencilAttachment = [this]{cleanUp(depth_stencil_type, &depth_buffer, 1);};

	unsigned int getID() const {
		return fbo;
	}

	unsigned int getColorID(int i = 0) const {
		return color_buffer[i];
	}

	unsigned int getDepthID() const {
		return depth_buffer;
	}

	unsigned int getStencilID() const {
		return stencil_buffer;
	}

	unsigned int getDepthStencilID() const {
		return depth_buffer;
	}

	int getWidth() const {
		return width;
	}

	int getHeight() const {
		return height;
	}

	AttachType getColorAttachType() const {
		return color_type;
	}

	AttachType getDepthAttacType() const {
		return depth_type;
	}

	AttachType getStencilAttachType() const {
		return stencil_type;
	}

	AttachType getDepthStencilAttachType() const {
		return depth_stencil_type;
	}
};

#endif