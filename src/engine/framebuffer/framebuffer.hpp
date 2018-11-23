#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

// This class represents opengl fbo

class FrameBuffer {
public:
	enum AttachType {
		NONE,
		TEXTURE,
		RBUFFER
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
	FrameBuffer(int width, int height, unsigned int numColorAttach = 1);
	~FrameBuffer();

	// set framebuffer attachments
	void setColorAttachment(AttachType type);
	void setDepthAttachment(AttachType type);
	void setStencilAttachment(AttachType type);
	void setDepthStencilAttachment(AttachType type);

	bool isComplete();

	void use(UseType type = NORMAL);
	static void useDefault();

	unsigned int getID() const {
		return fbo;
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