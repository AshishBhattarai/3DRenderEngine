#include "collision_shape.hpp"

#include "utils/slogger.hpp"

// sphere
CollisionShape::CollisionShape(float radius) :
	type(SPHERE)
{
	shape = new btSphereShape(btScalar(radius));
}

// cube
CollisionShape::CollisionShape(glm::vec3 extends) :
	type(CUBE)
{
	shape = new btBoxShape(btVector3(extends.x, extends.y, extends.z));
}

// static plane
CollisionShape::CollisionShape(glm::vec3 normal, float local_height) :
	type(PLANE)
{
	shape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), local_height);
}

// terrain
CollisionShape::CollisionShape(const std::vector<float>& heights, int size,
	int vertexCount, float maxHeight) :
	type(HEIGHT_FIELD)
{
	btHeightfieldTerrainShape* heightField = new btHeightfieldTerrainShape(
		vertexCount+1, vertexCount+1, &heights[0], 1, -maxHeight, maxHeight, 1,
		PHY_FLOAT, false
	);
	// heightField->setUseDiamondSubdivision(true);

	// scale heightField
	float ssize = (float)size/(vertexCount-1);
	heightField->setLocalScaling(btVector3(ssize, 1.0f, ssize));
	shape = heightField;
}

// improted
CollisionShape::CollisionShape(btCollisionShape* shape, Type type):
	shape(shape),
	type(type)
{}

CollisionShape::~CollisionShape() {
	// compound shapes childerns must be deleted manually
	if(type == COMPOUND_SHAPE) {
		btCompoundShape* cs = static_cast<btCompoundShape*>(shape);
		for(int i = 0; i < cs->getNumChildShapes(); ++i)
			delete cs->getChildShape(i);
	}

	delete shape;
}