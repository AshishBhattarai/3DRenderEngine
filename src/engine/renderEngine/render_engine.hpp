#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include <list>

#include "entities/entity.hpp"
#include "entities/camera.hpp"
#include "entities/light.hpp"
#include "entity_renderer.hpp"
#include "gui_renderer.hpp"
#include "skybox_renderer.hpp"
#include "shader/colored_entity_shader.hpp"
#include "shader/entity_shader.hpp"
#include "shader/terrain_shader.hpp"
#include "shader/gui_shader.hpp"
#include "shader/buffer/general_vs_ubo.hpp"
#include "shader/buffer/general_fs_ubo.hpp"
#include "terrain_renderer.hpp"
#include "entities/point_light.hpp"
#include "entities/lamp.hpp"
#include "utils/type_conversion.hpp"

// renders the world(everything) with help of other sub renderers
class Gui;
class Skybox;
class RenderEngine {
private:
	static constexpr float FOV = 75.0f;
	static constexpr float NEAR = 0.1f;
	static constexpr float FAR = 350.0f; //~380

	static constexpr float FOG_DENSITY = 0.0038f;
	static constexpr float FOG_GRADIENT = 20.0f;

	static constexpr float D_SCENE_AMBIENT = 0.2f;

	float scene_ambient;

	glm::mat4 projection_mat;
	std::array<glm::vec4, 6> frustum;
	glm::vec3 fogColor;

	// uniform buffers
	GeneralVSUBO vsUBO;
	GeneralFSUBO fsUBO;

	// careful with order shader should be defined/declared before renderer
	// entity
	ColoredEntityShader coloredEntityShader;
	EntityShader entityShader;
	BoundingBoxShader bbShader; // entity bounding box shader
	EntityRenderer entityRenderer;

	// Terrain
	TerrainShader terrainShader;
	TerrainRenderer terrainRenderer;

	// Gui
	Gui& gui;
	GuiShader guiShader;
	GuiRenderer guiRenderer;

	// Skybox
	Skybox* skybox;
	SkyboxShader skyboxShader;
	SkyboxRenderer skyboxRenderer;

	Camera* camera;
	Light* sun;

	// list to render
	std::vector<Terrain*> terrains;
	std::vector<Entity*> entities;
	std::vector<PointLight*> pointLights;

	void setProjectionMatrix();
	// load mat Uniform binding point to the shaders(list arguments)
	void setMatUniformBinding(std::initializer_list<Shader*> list);
	void setUniformBinding(const std::string& bname, GLuint bpoint, std::initializer_list<Shader*> list);
	void prepare(); // prepare for rendering
	void sortEntities();
	void clearRenderData();

public:
	RenderEngine(Camera* camera, Light* sun);
	~RenderEngine();

	// add entites to vector
	void processEntity(std::vector<std::unique_ptr<Entity>>& entities);

	// call at start for each frame
	void newFrame();

	// add Terrains to vector
	void processTerrain(Terrain* terrain) {
		terrains.emplace_back(terrain);
	}

	// reload sun data to ubo
	void reloadSun() {
		fsUBO.setSun(*sun);
	}

	// call renderers
	void render();

	void renderGui() {
		guiRenderer.render();
	}

	// setters
	void setSun(Light* sun) {
		this->sun = sun;
		reloadSun();
	}

	void setSkybox(Skybox* skybox) {
		this->skybox = skybox;
	}

	void setCamera(Camera* camera) {
		this->camera = camera;
	}

	void setFogColor(const glm::vec3& color) {
		fogColor = color;
		fsUBO.setFogColor(fogColor);
	}

	// TODO: Remove this later
	const glm::mat4& getProjectionMatrix() const {
		return projection_mat;
	}

	GLuint getGeneralVSBinding() const {
		return vsUBO.getBindingPoint();
	}

	float getSceneAmbient() const {
		return scene_ambient;
	}

	void setSceneAmbient(float val) {
		scene_ambient = val;
		fsUBO.setAmbient(val);
	}

	void addPointLight(PointLight* light);
};

#endif