#ifndef SKYBOX_SHADER_HPP
#define SKYBOX_SHADER_CPP

#include "shader.hpp"
#include <GLFW/glfw3.h>

class SkyboxShader : public Shader {
private:
	static constexpr std::string_view VERTEX_FILE = "shaders/skybox.vert";
	static constexpr std::string_view FRAGMENT_FILE = "shaders/skybox.frag";

	// skybox view * roation matrix
	GLint loc_skyboxMat;
	GLint loc_enableFog;

public:
	SkyboxShader();
	// view+rotation - rmp - rotation per min
	void loadSkyboxMatrix(const glm::mat4& view, float rmp = 0.0f);
	void loadEnableFog(bool enable);

};

#endif