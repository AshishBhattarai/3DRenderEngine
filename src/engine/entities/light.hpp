#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>

class Light {
protected:
	// attributes
	glm::vec3 position; // direction for direction light
	glm::vec3 diffuse;
	glm::vec3 specular;

public:
	Light(glm::vec3 position = glm::vec3(0.0f)) :
			position(position),
			diffuse(0.0f),
			specular(0.0f)
	{

	}

	// getters / setters
	glm::vec3 getPosition() const {
		return position;
	}

	glm::vec3 getDiffuse() const {
		return diffuse;
	}

	glm::vec3 getSpecular() const {
		return specular;
	}

	void setPosition(glm::vec3 pos) {
		position = pos;
	}

	void setDiffuse(glm::vec3 color) {
		diffuse = color;
	}

	void setSpecular(glm::vec3 color) {
		specular = color;
	}
};

#endif