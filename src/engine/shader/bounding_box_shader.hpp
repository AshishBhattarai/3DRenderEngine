#ifndef BOUNDING_BOX_SHADER_HPP
#define BOUNDING_BOX_SHADER_HPP

#include "shader.hpp"

#include <string_view>

class BoundingBoxShader : public Shader {
private:
	static constexpr std::string_view VERTEX_FILE = "shaders/bb.vert";
	static constexpr std::string_view FRAGMENT_FILE = "shaders/bb.frag";

public:
	BoundingBoxShader() :
		Shader(VERTEX_FILE, FRAGMENT_FILE)
	{}
};

#endif