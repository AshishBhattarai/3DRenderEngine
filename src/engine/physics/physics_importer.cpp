#include "physics_importer.hpp"

PhysicsImporter::PhysicsImporter() :
	btWorldImporter(0),
	bulletFile(nullptr)
{
	deleteAllData();
}

PhysicsImporter::~PhysicsImporter() {
	clearImport();
}

bool PhysicsImporter::parseFile(const char* file) {
	bulletFile = new bParse::btBulletFile(file);
	bool ok = (bulletFile->getFlags()& bParse::FD_OK)!=0;
	if (ok)
		bulletFile->parse(m_verboseMode);
	else
		return false;
	if (m_verboseMode & bParse::bFileVerboseMode::FD_VERBOSE_DUMP_CHUNKS)
	{
		bulletFile->dumpChunks(bulletFile->getFileDNA());
	}
	return true;
}

bool PhysicsImporter::import(const char* file) {
	clearImport();
	// parse the file
	return parseFile(file);
}

int PhysicsImporter::getNumCollisionShape() const {
	return bulletFile->m_collisionShapes.size();
}

std::shared_ptr<CollisionShape> PhysicsImporter::getCollisionShape(int index) {
	// get collisoin shape
	btCollisionShapeData* shapeData = (btCollisionShapeData*)bulletFile->m_collisionShapes[index];
	btCollisionShape* shape = convertCollisionShape(shapeData);
	return std::make_shared<CollisionShape>(shape, (CollisionShape::Type)shape->getShapeType());
}

void PhysicsImporter::clearImport() {
	if(bulletFile)
		delete bulletFile;
}