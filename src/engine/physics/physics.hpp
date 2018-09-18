#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

// singleton class represents physics world

class Physics {
private:
	// default physics values
	static constexpr float GRAVITY 		= -10.0f;
	static constexpr float MAX_STEPS	=	 6; // maxsubsteps per frame

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collConfig;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	Physics();
	~Physics();

public:
	// instance
	static Physics& getInstance() {
		static Physics instance;
		return instance;
	}

	// disable copying
	Physics(const Physics&) = delete;
	Physics& operator=(const Physics&) = delete;

	void processPhysics(float delta) {
		dynamicsWorld->stepSimulation(delta, MAX_STEPS);
	}

	void addRigidBody(btRigidBody* body) {
		dynamicsWorld->addRigidBody(body);
	}

	void removePhyiscBody(btCollisionObject* physicsBody) {
		dynamicsWorld->removeCollisionObject(physicsBody);
	}

	void setDebugDrawer(btIDebugDraw* drawer) {
		dynamicsWorld->setDebugDrawer(drawer);
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

};

#endif