#ifndef PHYSICS_IMPORTER_HPP
#define PHYSICS_IMPORTER_HPP

#include <bullet/BulletWorldImporter/btWorldImporter.h>
#include <bullet/BulletFileLoader/btBulletFile.h>
#include <memory>

#include "collision_shape.hpp"
#include "rigid_body.hpp"

class PhysicsImporter : private btWorldImporter {
private:
	bParse::btBulletFile* bulletFile;

	PhysicsImporter();
	~PhysicsImporter();

	bool parseFile(const char* file);

public:
	// singleton
	static PhysicsImporter& getInstance() {
		static PhysicsImporter instance;
		return instance;
	}
	PhysicsImporter(PhysicsImporter&) = delete;
	PhysicsImporter& operator=(PhysicsImporter&) = delete;

	// import phsyics data
	bool import(const char* file);

	int getNumCollisionShape() const;

	// get collision shape from import
	std::shared_ptr<CollisionShape> getCollisionShape(int index = 0);

	// clear/delete after import
	void clearImport();
};

#endif