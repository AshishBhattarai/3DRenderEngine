#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "entities/entity.hpp"

#include "shader/entity_shader.hpp"

// This class renders entities in batch

class EntityRenderer {
public:
	using EntityListMap = std::unordered_map<Model*, std::vector<Entity*>>;
private:
	EntityShader* shader;

	void loadTransformation(const Entity& entity);
	void prepareMesh(const TexturedMesh& mesh);

public:
	EntityRenderer(EntityShader& shader);

	void render(EntityListMap& entities);
};

#endif