#ifndef PHYSICS_TERRAIN_HPP
#define PHYSICS_TERRAIN_HPP

#include "terrain/terrain.hpp"

#include <memory>

#include "physics/collision_shape/collision_shape.hpp"
#include "physics/physics_body/static_body.hpp"

class PhysicsTerrain : public Terrain {
private:
	std::unique_ptr<StaticBody> physicsBody;

	/* creates height field collision shape and inits physicsBody */
	void initPhysicsBody(float x, float z);

public:
	PhysicsTerrain(unsigned int gridX, unsigned int gridZ, int size, float maxHeight, Image& heightMap,
		Texture::Map& textures);
	PhysicsTerrain(unsigned int gridX, unsigned int gridZ, int size, unsigned int vertexCount, Texture::Map& textures);

	StaticBody* getPhysicsBody() const;
};

#endif