#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

// singleton class represents physics world

class DebugDrawer;
class PhysicsWorld {
private:
	// default physics values
	static constexpr float GRAVITY 		= -10.0f;
	static constexpr float MAX_STEPS	=	 6; // maxsubsteps per frame

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collConfig;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	DebugDrawer* debug_drawer;

	PhysicsWorld();
	~PhysicsWorld();

public:
	// instance
	static PhysicsWorld& getInstance() {
		static PhysicsWorld instance;
		return instance;
	}

	// disable copying
	PhysicsWorld(const PhysicsWorld&) = delete;
	PhysicsWorld& operator=(const PhysicsWorld&) = delete;

	void processPhysics(float delta) {
		dynamicsWorld->stepSimulation(delta, MAX_STEPS);
	}

	void addRigidBody(btRigidBody* body) {
		dynamicsWorld->addRigidBody(body);
	}

	void addCollisionObject(btCollisionObject* object) {
		dynamicsWorld->addCollisionObject(object);
	}

	void removePhyiscBody(btCollisionObject* physicsBody) {
		dynamicsWorld->removeCollisionObject(physicsBody);
	}

	void updateSingleAABB(btCollisionObject* physicsBody) {
		dynamicsWorld->updateSingleAabb(physicsBody);
	}

	void debugDraw() {
		dynamicsWorld->debugDrawWorld();
	}

	void setGravity(const glm::vec3& gravity) {
		dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	}

	glm::vec3 getGravity() const {
		btVector3 rg = dynamicsWorld->getGravity();
		return glm::vec3(rg.x(), rg.y(), rg.z());
	}

	DebugDrawer* getDebugDrawer() const;

	void setVSDataBinding(unsigned int point);
};

#endif