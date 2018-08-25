#include "texture.hpp"

#include "utils/image.hpp"

void Texture::loadTexture(const std::string& path,  bool flip_y) {
	// load image
	Image image(path, flip_y);

	// type
	GLenum type;
	int nrChannels = image.getNumChannels();
	if(nrChannels == 1)
		type = GL_RED;
	else if(nrChannels == 3)
		type = GL_RGB;
	else
		type = GL_RGBA;

	// setup texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	// setup parameter
	setTextureWrap(GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lod_bias);

	// load image
	glTexImage2D(GL_TEXTURE_2D, 0, type, image.getWidth(), image.getHeight(), 0,
			type, GL_UNSIGNED_BYTE, image.getData());
	// mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string& path, Type type, bool flip_y, float lod_bias) :
	type(type),
	lod_bias(lod_bias),
	id(0)
{
	loadTexture(path, flip_y);
}

Texture::~Texture() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &id);
}

void Texture::setLOD(float lod_bias) {
	this->lod_bias = lod_bias;
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lod_bias);
}

void Texture::setTextureWrap(GLenum wrap) {
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}