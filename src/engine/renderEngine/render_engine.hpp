#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include <list>

#include "entities/entity.hpp"
#include "entities/camera.hpp"
#include "entities/light.hpp"
#include "entity_renderer.hpp"
#include "shader/colored_entity_shader.hpp"
#include "shader/entity_shader.hpp"
#include "shader/terrain_shader.hpp"
#include "shader/buffer/general_vs_ubo.hpp"
#include "shader/buffer/general_fs_ubo.hpp"
#include "terrain_renderer.hpp"
#include "render_filter.hpp"
#include "utils/type_conversion.hpp"

// renders the world(everything) with help of other sub renderers

class RenderEngine {
private:
	static constexpr float FOV = 75.0f;
	static constexpr float NEAR = 0.1f;
	static constexpr float FAR = 350.0f; //~380

	static constexpr float FOG_DENSITY = 0.0038f;
	static constexpr float FOG_GRADIENT = 10.0f;

	glm::mat4 projection_mat;
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

	Camera* camera;
	Light* sun;

	RenderFilter renderFilter;
	btDbvt* dbvt;

	// list to render
	std::vector<Terrain*> terrains;
	std::vector<Entity*> entities;

	void setProjectionMatrix();
	// load mat Uniform binding point to the shaders(list arguments)
	void setMatUniformBinding(std::initializer_list<Shader*> list);
	void setUniformBinding(const std::string& bname, u_int bpoint, std::initializer_list<Shader*> list);
	void prepare(); // prepare for rendering
	void frustumCull();
	void sortEntities();
	void clearRenderData();

public:
	RenderEngine(Camera* camera, Light* sun);
	~RenderEngine();

	// add entites to map
	void processEntity(std::vector<std::unique_ptr<Entity>>& entities) {
		for(auto& e : entities) {
			Entity* entity = e.get();
			// create a broadphase
			btVector3 minBB = VEC3::glmToBt(entity->getMinBB());
			btVector3 maxBB = VEC3::glmToBt(entity->getMaxBB());
			// this->entities.push_back(entity);
			dbvt->insert(btDbvtVolume::FromMM(minBB, maxBB), static_cast<void*>(entity));
		}
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