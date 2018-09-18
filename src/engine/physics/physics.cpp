#include "physics.hpp"

Physics::Physics() {
	// init physics world
	broadphase = new btDbvtBroadphase();
	collConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collConfig);
	solver = new btSequentialImpulseConstraintSolver();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collConfig);
	dynamicsWorld->setGravity(btVector3(0.0, GRAVITY, 0.0f));
}

Physics::~Physics() {
	// clearnup
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete broadphase;
	delete collConfig;
}