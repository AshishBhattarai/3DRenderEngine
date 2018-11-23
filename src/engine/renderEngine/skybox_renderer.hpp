#ifndef SKYBOX_RENDERER_HPP
#define SKYBOX_RENDERER_HPP

#include "model/skybox.hpp"
#include "shape/shape_mesh.hpp"
#include "shader/skybox_shader.hpp"

#include <glad/glad.h>

class SkyboxRenderer {
private:
	SkyboxShader* shader;
	// render data
	std::unique_ptr<ShapeMesh> skybox_mesh;

public:
	SkyboxRenderer(SkyboxShader& shader);
	~SkyboxRenderer();

	void render(Skybox* skybox);

};

#endif