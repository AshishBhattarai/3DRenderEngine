#include "render_engine.hpp"

#include "display_manager.hpp"
#include "shader/shader_config.hpp"
#include "opengl_query.hpp"
#include "gui/gui.hpp"
#include "model/skybox.hpp"

#include "utils/math.hpp"

RenderEngine::RenderEngine(Camera* camera, Light* sun):
		projection_mat(1.0f),
		fogColor(1.0f, 1.0f, 1.0f),
		vsUBO(),
		fsUBO(),
		coloredEntityShader(),
		entityShader(),
		bbShader(),
		entityRenderer(entityShader, coloredEntityShader, bbShader),
		terrainShader(),
		terrainRenderer(terrainShader),
		gui(Gui::getGui()), // must be called before guiRenderer
		guiShader(),
		guiRenderer(guiShader),
		skybox(nullptr),
		skyboxShader(),
		skyboxRenderer(skyboxShader),
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
		{&coloredEntityShader, &entityShader, &terrainShader, &bbShader, &skyboxShader}
	);
	setUniformBinding(
		ShaderConfig::GeneralFSData,
		fsUBO.getBindingPoint(),
		{&coloredEntityShader, &entityShader, &terrainShader, &skyboxShader}
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
	float width = (float)display.getScreenWidth();
	float height = (float)display.getScreenHeight();

	projection_mat = glm::perspective(glm::radians(FOV), width / height, NEAR, FAR);
	// load projection matrix to uniform
	vsUBO.setProjectionMatrix(projection_mat);

	// ortho graphic projection
	guiShader.start();
	guiShader.loadProjectionMatrix(glm::ortho(0.0f, width, height, 0.0f));
	guiShader.stop();
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

void RenderEngine::clearRenderData() {
	entities.clear(); // clear entity_map
	terrains.clear(); // clear Terrain

	// clear dbvt
	dbvt->clear();
}

void RenderEngine::render() {
	// optimize
	frustumCull();
	sortEntities();

	// update global uniforms
	vsUBO.setViewMatrix(camera->getViewMatrix());
	vsUBO.setCameraPos(camera->getPosition());

	// start render
	prepare();
	// render terrain
	terrainRenderer.render(terrains);
	// render entity
	entityRenderer.render(entities);
	// render skybox
	if(skybox) {
		skyboxRenderer.render(skybox);
		skyboxShader.loadSkyboxMatrix(camera->getViewMatrix(), skybox->getRPM());
	}
	clearRenderData();
}

void RenderEngine::newFrame() {
	gui.newFrame();
}