#ifndef SKYBOX_RENDERER_HPP
#define SKYBOX_RENDERER_HPP

#include "model/skybox.hpp"
#include "shader/skybox_shader.hpp"

#include <glad/glad.h>

class SkyboxRenderer {
private:
	SkyboxShader* shader;
	// render data
	GLuint vao, vbo;

public:
	SkyboxRenderer(SkyboxShader& shader);
	~SkyboxRenderer();

	void render(Skybox* skybox);

};

#endif