#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <string>
#include "model/texture.hpp"

struct SkyboxPath {
	std::string right;
	std::string left;
	std::string top;
	std::string bottom;
	std::string front;
	std::string back;
};

class Skybox {
private:
	Texture texture;
	float rpm; // rotation per min

public:
	Skybox(SkyboxPath& path, float rpm = 0.0f);

public:
	GLuint getCubemapID() const;
	float getRPM() const;
};



#endif