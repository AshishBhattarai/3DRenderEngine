#include "skybox.hpp"

Skybox::Skybox(SkyboxPath& path, float rpm) :
		texture(std::array<std::string, Texture::CUBEMAP_NUM_FACES>{
			path.left, path.right, path.top, path.bottom, path.front, path.back
		}),
		rpm(rpm),
		lowerFogLimit(0.0f),
		upperFogLimit(0.1f),
		fogEnable(true)
	{}

GLuint Skybox::getCubemapID() const {
	return texture.getID();
}

float Skybox::getRPM() const {
	return rpm;
}

void Skybox::setFogEnable(bool enable) {
	fogEnable = enable;
}

bool Skybox::getFogEnable() const {
	return fogEnable;
}