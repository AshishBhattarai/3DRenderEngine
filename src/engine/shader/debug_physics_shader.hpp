#ifndef DEBUG_PHYSICS_SHADER_HPP
#define DEBUG_PHYSICS_SHADER_HPP

#include "shader.hpp"

#include <string_view>

class DebugPhysicsShader : public Shader {

private:
	static constexpr std::string_view VERTEX_FILE = "shaders/physics.vert";
	static constexpr std::string_view FRAGMENT_FILE = "shaders/physics.frag";

	GLint loc_isContactPoint;

public:
	DebugPhysicsShader();

	void setIsContactPoing(bool val) {
		uniform1i(loc_isContactPoint, val);
	}
};

#endif