#include "render_engine.hpp"

#include "display_manager.hpp"
#include "shader/shader_config.hpp"
#include "opengl_query.hpp"

#include "utils/math.hpp"

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
		bbShader(),
		bbRenderer(bbShader),
		camera(camera),
		sun(sun),
		renderFilter(entities, terrains),
		dbvt(new btDbvt)
{
	// reserve space
	entities.reserve(1000);
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
		{&coloredEntityShader, &entityShader, &terrainShader, &bbShader}
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

void RenderEngine::setProjectionMatrix() {
	// setup projection matrix
	DisplayManager& display = DisplayManager::getInstance();
	projection_mat = glm::perspective(glm::radians(FOV),
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

void RenderEngine::sortEntities() {
	// sort entites
	std::sort(entities.begin(), entities.end(), [&camera = camera](Entity* e1, Entity* e2)->bool {
		glm::vec3 cp = camera->getPosition();
		glm::vec3 cz = camera->getFrontDirection();
		glm::vec3 x1 = e1->getPosition();
		glm::vec3 x2 = e2->getPosition();

		float dst1 = glm::dot(x1 - cp, cz);
		float dst2 = glm::dot(x2 - cp, cz);

		return dst1 > dst2; // front to back
	});
}

void RenderEngine::occlusionCull(std::vector<Entity*>& entity_list) {
	// get all the visable entities only
	std::for_each(entities.begin(), entities.end(), [&entity_list](Entity* e) {
		if(e->getOcclusionQuery()->getResult())
			entity_list.push_back(e);
	});
}

void RenderEngine::drawEntityBB() {
	// disable writing to frame buffer
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// remove all visible entites
	std::remove_if(entities.begin(), entities.end(), [](Entity* e)->bool {
		return e->getOcclusionQuery()->getLastResult();
	});

	bbRenderer.render(entities);

	// enable again
	glDepthMask(GL_TRUE);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void RenderEngine::clearRenderData() {
	entities.clear(); // clear entity_map
	terrains.clear(); // clear Terrain

	// clear dbvt
	dbvt->clear();
}

void RenderEngine::render() {
	// optimize
	std::vector<Entity*> filtered_entities;
	frustumCull();
	sortEntities();
	occlusionCull(filtered_entities);

	// update global uniforms
	vsUBO.setViewMatrix(camera->getViewMatrix());
	vsUBO.setCameraPos(camera->getPosition());

	// start render
	prepare();
	// render terrain
	terrainRenderer.render(terrains);
	// render entity
	entityRenderer.render(filtered_entities);
	// Note:: drawEntityBB() removes all the visble entities from the vector(entities).
	drawEntityBB();

	clearRenderData();
}