#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include "entities/entity.hpp"
#include "entities/camera.hpp"
#include "entities/light.hpp"
#include "entity_renderer.hpp"
#include "shader/entity_shader.hpp"
#include "shader/terrain_shader.hpp"
#include "shader/buffer/general_vs_ubo.hpp"
#include "shader/buffer/general_fs_ubo.hpp"
#include "terrain_renderer.hpp"

// renders the world(everything) with help of other sub renderers

class RenderEngine {
private:
	static constexpr float FOV = 45.0f;
	static constexpr float NEAR = 0.1f;
	static constexpr float FAR = 800.0f;

	static constexpr float FOG_DENSITY = 0.0038f;
	static constexpr float FOG_GRADIENT = 10.0f;

	glm::mat4 projection_mat;
	glm::vec3 fogColor;

	// uniform buffers
	GeneralVSUBO vsUBO;
	GeneralFSUBO fsUBO;

	// careful with order shader should be defined/declared before renderer
	// entity
	EntityShader entityShader;
	EntityRenderer entityRenderer;

	// Terrain
	TerrainShader terrainShader;
	TerrainRenderer terrainRenderer;

	Camera* camera;
	Light* sun;

	// list to render
	std::vector<Terrain*> terrains;
	EntityRenderer::EntityListMap entities;

	void setProjectionMatrix();
	// load mat Uniform binding point to the shaders(list arguments)
	void setMatUniformBinding(std::initializer_list<Shader*> list);
	void setUniformBinding(const std::string& bname, u_int bpoint, std::initializer_list<Shader*> list);
	void prepare(); // prepare for rendering

public:
	RenderEngine(Camera* camera, Light* sun);

	// add entites to map
	void processEntity(Entity* entity) {
		Model* model = entity->getModel(); // get model
		entities[model].emplace_back(entity); // add to the map
	}

	// add Terrains to map
	void processTerrain(Terrain* terrain) {
		terrains.emplace_back(terrain);
	}

	// reload sun data to ubo
	void reloadSun() {
		fsUBO.setSun(*sun);
	}

	// call renderers
	void render();

	// setters
	void setSun(Light* sun) {
		this->sun = sun;
		reloadSun();
	}

	void setCamera(Camera* camera) {
		this->camera = camera;
	}

	// TODO: Remove this later
	const glm::mat4& getProjectionMatrix() const {
		return projection_mat;
	}

	GLuint getGeneralVSBinding() const {
		return vsUBO.getBindingPoint();
	}
};

#endif