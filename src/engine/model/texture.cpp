#include "texture.hpp"

#include "utils/image.hpp"

Texture::Texture(const std::string& path, Type type, bool flip_y, float lod_bias) :
	id(0),
	type(type),
	lod_bias(lod_bias)
{
	loadTexture(path, flip_y);
}

Texture::Texture(const std::array<std::string, CUBEMAP_NUM_FACES>& paths, bool flip_y) :
	id(0),
	type(DIFFUSE_CUBE_MAP),
	lod_bias(0)
{
	loadCubeMap(paths, flip_y);
}

Texture::~Texture() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &id);
}

bool Texture::converSRGB() {
	if(type == DIFFUSE_MAP || type == DIFFUSE_BLACK_MAP ||
			type == DIFFUSE_RED_MAP || type == DIFFUSE_GREEN_MAP ||
			type == DIFFUSE_BLUE_MAP || type == EMISSION_MAP ||
			type == DIFFUSE_CUBE_MAP)
		return true;
	return true;
}

void Texture::loadTexture(const std::string& path,  bool flip_y) {
	// load image
	Image image(path, flip_y);
	if(image.failed()) return;

	// color types
	GLenum typeA, typeB;
	int nrChannels = image.getNumChannels();
	typeB = typeA = (nrChannels == 4)? GL_RGBA : GL_RGB;

	// check if texture needs to converted to RGB from sRGB
	if(converSRGB())
		typeA = (typeA == GL_RGB)? GL_SRGB : GL_SRGB_ALPHA;

	// setup texture
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	// setup parameter
	setTextureWrap(GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lod_bias);

	// load image
	glTexImage2D(GL_TEXTURE_2D, 0, typeA, image.getWidth(), image.getHeight(), 0,
		typeB, GL_UNSIGNED_BYTE, image.getData());
	// mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadCubeMap(const std::array<std::string, CUBEMAP_NUM_FACES>& paths, bool flip_y) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	// iterate through each face
	for(int i = 0; i < CUBEMAP_NUM_FACES; ++i) {
		// load image
		Image image(paths[i], flip_y);
		if(image.failed())
			continue;

	// color types
	GLenum typeA, typeB;
	int nrChannels = image.getNumChannels();
	typeB = typeA = (nrChannels == 4)? GL_RGBA : GL_RGB;

	// check if texture needs to converted to RGB from sRGB
	if(converSRGB())
		typeA = (typeA == GL_RGB)? GL_SRGB : GL_SRGB_ALPHA;

		// load texture to cubemap
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, typeA,
			image.getWidth(), image.getHeight(), 0, typeB, GL_UNSIGNED_BYTE, image.getData());
	}

  // texture parameters for the cubemap
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture::setLOD(float lod_bias) {
	if(type == DIFFUSE_CUBE_MAP) return;

	this->lod_bias = lod_bias;
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lod_bias);
}

void Texture::setTextureWrap(GLenum wrap) {
	if(type == DIFFUSE_CUBE_MAP) return;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
}