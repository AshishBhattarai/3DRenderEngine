#include "physics_world.hpp"

#include "debug_drawer.hpp"

PhysicsWorld::PhysicsWorld()
{
	// init physics world
	broadphase = new btDbvtBroadphase();
	collConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collConfig);
	solver = new btSequentialImpulseConstraintSolver();

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collConfig);
	dynamicsWorld->setGravity(btVector3(0.0, GRAVITY, 0.0f));

	debug_drawer = new DebugDrawer();
	dynamicsWorld->setDebugDrawer(debug_drawer);
}

PhysicsWorld::~PhysicsWorld() {
	// clearnup
	dynamicsWorld->setDebugDrawer(0);
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete broadphase;
	delete collConfig;
	delete debug_drawer;
}

DebugDrawer* PhysicsWorld::getDebugDrawer() const {
	return debug_drawer;
}

void PhysicsWorld::setVSDataBinding(unsigned int point) {
	debug_drawer->setVSDataBinding(point);
}