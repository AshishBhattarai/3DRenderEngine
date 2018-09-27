#include "physics_terrain.hpp"

void PhysicsTerrain::initPhysicsBody(float x, float z) {
	// terrain height field
	std::shared_ptr<CollisionShape>heightField
		= std::make_shared<CollisionShape>(heights, size, vertexCount, maxHeight);

	// physics body position
	float offset = gridSize/2.0f;
	x = ((x+1) * size)/2.0f + offset;
	z = ((z+1) * size)/2.0f + offset;

	// static body
	physicsBody = std::make_unique<StaticBody>(heightField, glm::vec3(x, 0.0f, z));
	physicsBody->disableDebugDraw();
}

PhysicsTerrain::PhysicsTerrain(u_int gridX, u_int gridZ, int size, u_int vertexCount, Texture::Map& textures) :
	Terrain(gridX, gridZ, size, vertexCount, textures)
{
	initPhysicsBody(gridX, gridZ);
}

PhysicsTerrain::PhysicsTerrain(u_int gridX, u_int gridZ, int size,
	float maxHeight, Image& heightMap, Texture::Map& textures) :
	Terrain(gridX, gridZ, size, maxHeight, heightMap, textures)
{
	initPhysicsBody(gridX, gridZ);
}

StaticBody* PhysicsTerrain::getPhysicsBody() const {
	return physicsBody.get();
}