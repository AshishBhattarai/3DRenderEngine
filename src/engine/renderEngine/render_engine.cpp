#include "render_engine.hpp"

#include "display_manager.hpp"
#include "shader/shader_config.hpp"

void RenderEngine::setProjectionMatrix() {
	// setup projection matrix
	DisplayManager& display = DisplayManager::getInstance();
	projection_mat = glm::perspective(glm::radians(45.0f),
		(float)display.getScreenWidth() / (float)display.getScreenHeight(), NEAR, FAR);

	// load projection matrix to uniform
	vsUBO.setProjectionMatrix(projection_mat);
}

void RenderEngine::setUniformBinding(const std::string& bname, u_int bpoint,
	std::initializer_list<Shader*> list)
{
	for(auto& s : list) {
		s->setUniformBlockBinding(bname.c_str(), bpoint);
	}
}

void RenderEngine::prepare() {
	glEnable(GL_CULL_FACE);
	glClearColor(fogColor.r, fogColor.g, fogColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

RenderEngine::RenderEngine(Camera* camera, Light* sun):
		projection_mat(1.0f),
		entityShader(),
		terrainShader(),
		terrainRenderer(terrainShader),
		entityRenderer(entityShader),
		camera(camera),
		sun(sun),
		fogColor(0.1f, 0.8f, 0.8f)
{
	// reserve space
	entities.reserve(400);
	terrains.reserve(20);

	setProjectionMatrix();

	// TODO: create a seperate fog class
	fsUBO.setFogColor(fogColor);
	fsUBO.setSun(*sun);

	// set uniform block binding point
	setUniformBinding(ShaderConfig::GeneralVSData, vsUBO.getBindingPoint(), {&entityShader, &terrainShader});
	setUniformBinding(ShaderConfig::GeneralFSData, fsUBO.getBindingPoint(), {&entityShader, &terrainShader});

	// enables
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void RenderEngine::render() {
	prepare();

	// update global uniforms
	vsUBO.setViewMatrix(camera->getViewMatrix());
	vsUBO.setCameraPos(camera->getPosition());

	// render entity
	entityShader.start();
	entityRenderer.render(entities);
	entityShader.stop();

	// enable again as some entites disable cull face
	glEnable(GL_CULL_FACE);

	// render terrain
	terrainShader.start();
	terrainRenderer.render(terrains);
	terrainShader.stop();

	entities.clear(); // clear entity_map
	terrains.clear(); // clear Terrain
}