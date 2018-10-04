#ifndef GUI_SHADER_HPP
#define GUI_SHADER_HPP

#include "shader.hpp"

class GuiShader : public Shader {
private:
	static constexpr std::string_view VERTEX_FILE = "shaders/gui.vert";
	static constexpr std::string_view FRAGMENT_FILE = "shaders/gui.frag";

	// orthographic projection loc
	GLint loc_projMat;

public:
	GuiShader();

	void loadProjectionMatrix(const glm::mat4& matrix) {
		uniformMatrix4fv(loc_projMat, matrix);
	}

};

#endif