#include "render_engine.hpp"

#include "display_manager.hpp"
#include "shader/shader_config.hpp"
#include "opengl_query.hpp"
#include "gui/gui.hpp"
#include "model/skybox.hpp"

#include "utils/math.hpp"

RenderEngine::RenderEngine(Camera* camera, Light* sun):
		scene_ambient(D_SCENE_AMBIENT),
		numPointLights(0),
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
		sun(sun)
{
	// reserve space
	entities.reserve(1000);
	terrains.reserve(20);
	// set all point lights to 0
	for(int i = 0; i < ShaderConfig::MAX_POINT_LIGHTS; ++i) {
		fsUBO.setPointLight(PointLight(), i);
	}

	setProjectionMatrix();
	frustum = Math::extractViewFrustum(projection_mat, camera->getViewMatrix());

	vsUBO.setFogProperty(FOG_DENSITY, FOG_GRADIENT);

	// TODO: create a seperate fog class
	fsUBO.setFogColor(fogColor);
	fsUBO.setAmbient(scene_ambient);
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

RenderEngine::~RenderEngine() {}

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

void RenderEngine::setUniformBinding(const std::string& bname, GLuint bpoint,
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

void RenderEngine::sortEntities() {
	// sort entites
	std::sort(entities.begin(), entities.end(), [&camera = camera](Entity* e1, Entity* e2)->bool {
		glm::vec3 cp = camera->getPosition();
		glm::vec3 cz = camera->getFrontDirection();
		glm::vec3 x1 = e1->getPosition();
		glm::vec3 x2 = e2->getPosition();

		// calculate entites z position WRT camera
		float z1 = glm::dot(x1 - cp, cz);
		float z2 = glm::dot(x2 - cp, cz);

		return z1 > z2; // front to back
	});
}

void RenderEngine::clearRenderData() {
	entities.clear(); // clear entity_vector
	terrains.clear(); // clear Terrain

	// clear pointLights
	for(unsigned int i = 0; i < numPointLights; i++)
		fsUBO.setPointLight(PointLight(), i);
	numPointLights = 0;
}

void RenderEngine::render() {
	// optimize
	sortEntities();

	// update global uniforms
	vsUBO.setViewMatrix(camera->getViewMatrix());
	vsUBO.setCameraPos(camera->getPosition());
	fsUBO.setNumPointLight(numPointLights);

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
	// calculate frustum planes
	frustum = Math::extractViewFrustum(projection_mat, camera->getViewMatrix());
}

	// add entites to vector
void RenderEngine::processEntity(std::vector<std::unique_ptr<Entity>>& entities) {
	for(auto& e : entities) {
		Entity* entity = e.get();

		// check if inside view frustum
		if(entity->getAABB().inFrustum(frustum))
			this->entities.push_back(entity);

		// check if lamp
		if(entity->getType() == Entity::LAMP)
			addPointLight(static_cast<PointLight*>(static_cast<Lamp*>(entity)));
	}
}

void RenderEngine::addPointLight(PointLight* light) {
	if(numPointLights >= ShaderConfig::MAX_POINT_LIGHTS) return;
	fsUBO.setPointLight(*light, numPointLights);
	++numPointLights;
}
