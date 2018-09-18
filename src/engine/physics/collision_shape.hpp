#ifndef COLLISION_SHAPE_HPP
#define COLLISION_SHAPE_HPP

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <glm/glm.hpp>
#include <vector>

class CollisionShape {
friend class RigidBody;
public:
	enum Type {
		SPHERE					=		SPHERE_SHAPE_PROXYTYPE,
		CUBE						=		BOX_SHAPE_PROXYTYPE,
		CONE						=		CONE_SHAPE_PROXYTYPE,
		CYLINDER				=		CYLINDER_SHAPE_PROXYTYPE,
		PLANE						=		STATIC_PLANE_PROXYTYPE,
		HEIGHT_FIELD		= 	TERRAIN_SHAPE_PROXYTYPE,
		CONVEX_HULL			=		CONVEX_HULL_SHAPE_PROXYTYPE,
		COMPOUND_SHAPE	=		COMPOUND_SHAPE_PROXYTYPE
	};

protected:
	btCollisionShape* shape;
	Type type;
	// for childs
	CollisionShape(Type type) {}

public:
	// sphere
	CollisionShape(float radius);
	// cube
	CollisionShape(glm::vec3 extends);
	// static plane
	CollisionShape(glm::vec3 normal, float local_height);
	// terrain heightfield
	CollisionShape(const std::vector<float>& heights, int size, int vertexCount, float maxHeight);
	// imported
	CollisionShape(btCollisionShape* shape, Type type);

	~CollisionShape();

	Type getShapeType() const {
		return type;
	}

	void setLocalScale(const glm::vec3& scale) {
		shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	}
};

#endif