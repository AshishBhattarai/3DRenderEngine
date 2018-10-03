#ifndef PHYSICS_IMPORTER_HPP
#define PHYSICS_IMPORTER_HPP

#include <bullet/BulletWorldImporter/btWorldImporter.h>
#include <bullet/BulletFileLoader/btBulletFile.h>
#include <memory>

#include "collision_shape/collision_shape.hpp"
#include "physics_body/rigid_body.hpp"

class PhysicsImporter : private btWorldImporter {
private:
	bParse::btBulletFile* bulletFile;

	bool parseFile(const char* file);

public:
	PhysicsImporter();
	~PhysicsImporter();

	// import phsyics data
	bool import(const char* file);

	int getNumCollisionShapes() const;

	// get collision shape from import
	std::shared_ptr<CollisionShape> getCollisionShape(int index = 0);

	// clear/delete after import
	void clearImport();
};

#endif