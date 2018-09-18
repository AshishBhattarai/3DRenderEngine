#include "debug_physics_shader.hpp"

DebugPhysicsShader::DebugPhysicsShader() :
	Shader(VERTEX_FILE, FRAGMENT_FILE)
{
	loc_isContactPoint = getUniformLoc("isContactPoint");
}