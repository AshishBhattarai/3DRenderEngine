#ifndef BOUNDING_BOX_RENDERER_HPP
#define BOUNDING_BOX_RENDERER_HPP

#include "entities/entity.hpp"
#include "shader/bounding_box_shader.hpp"

class BoundingBoxRenderer {
private:
	BoundingBoxShader& shader;

public:
	BoundingBoxRenderer(BoundingBoxShader& shader);
	void render(std::vector<Entity*>& entities);
};

#endif