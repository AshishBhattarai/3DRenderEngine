#include "render_engine.hpp"

#include "display_manager.hpp"
#include "shader/shader_config.hpp"

#include "utils/math.hpp"

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

void RenderEngine::frustumCull() {
	// perform frustum culling
	std::array<glm::vec4, 6> frustum = Math::extractViewFrustum(projection_mat, camera->getViewMatrix());
	btVector3 plane_normals[6];
	btScalar plane_offsets[6];

	for(int i = 0; i < 6; ++i) {
		// frustum[i] = glm::normalize(frustum[i]);
		plane_normals[i] = btVector3(frustum[i].x, frustum[i].y, frustum[i].z);
		plane_offsets[i] = btScalar(frustum[i].w);
	}

	btDbvt::collideKDOP(dbvt->m_root, plane_normals, plane_offsets, 6, renderFilter);
}

void RenderEngine::clearRenderData() {
	entities.clear(); // clear entity_map
	terrains.clear(); // clear Terrain

	// clear dbvt
	if(dbvt) {
		delete dbvt;
		dbvt = new btDbvt; // new dbvt for next frame
	}
}

RenderEngine::RenderEngine(Camera* camera, Light* sun):
		projection_mat(1.0f),
		fogColor(0.1f, 0.8f, 0.8f),
		vsUBO(),
		fsUBO(),
		coloredEntityShader(),
		entityShader(),
		entityRenderer(entityShader, coloredEntityShader),
		terrainShader(),
		terrainRenderer(terrainShader),
		camera(camera),
		sun(sun),
		renderFilter(entities, terrains),
		dbvt(new btDbvt)
{
	// reserve space
	entities.reserve(400);
	terrains.reserve(20);

	setProjectionMatrix();
	vsUBO.setFogProperty(FOG_DENSITY, FOG_GRADIENT);

	// TODO: create a seperate fog class
	fsUBO.setFogColor(fogColor);
	fsUBO.setSun(*sun);

	// set uniform block binding point
	setUniformBinding(
		ShaderConfig::GeneralVSData,
		vsUBO.getBindingPoint(),
		{&coloredEntityShader, &entityShader, &terrainShader}
	);
	setUniformBinding(
		ShaderConfig::GeneralFSData,
		fsUBO.getBindingPoint(),
		{&coloredEntityShader, &entityShader, &terrainShader}
	);

	// enables
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

RenderEngine::~RenderEngine() {
	delete dbvt;
}

void RenderEngine::render() {
	prepare();
	frustumCull();

	// update global uniforms
	vsUBO.setViewMatrix(camera->getViewMatrix());
	vsUBO.setCameraPos(camera->getPosition());

	// render entity
	entityRenderer.render(entities);

	// render terrain
	terrainRenderer.render(terrains);

	clearRenderData();
}