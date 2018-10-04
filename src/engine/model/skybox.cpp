#include "skybox.hpp"

Skybox::Skybox(SkyboxPath& path, float rpm) :
		texture(std::array<std::string, Texture::CUBEMAP_NUM_FACES>{
			path.left, path.right, path.top, path.bottom, path.front, path.back
		}),
		rpm(rpm)
	{}

GLuint Skybox::getCubemapID() const {
	return texture.getID();
}

float Skybox::getRPM() const {
	return rpm;
}