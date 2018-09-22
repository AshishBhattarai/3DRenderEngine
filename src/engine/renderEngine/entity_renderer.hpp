#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

#include "entities/entity.hpp"

#include "shader/colored_entity_shader.hpp"
#include "shader/entity_shader.hpp"

// This class renders entities in batch

class EntityRenderer {
public:
	using EntityListMap = std::unordered_map<Model*, std::vector<Entity*>>;
private:
	EntityShader* entityShader;
	ColoredEntityShader* coloredEntityShader;

	void loadTransformation(const Entity& entity, Shader* shader);
	void prepareMesh(const TexturedMesh& mesh);
	void prepareMesh(const MaterialMesh& mesh);

public:
	EntityRenderer(EntityShader& entityShader, ColoredEntityShader& coloredEntityShader);

	void render(EntityListMap& entities);
};

#endif