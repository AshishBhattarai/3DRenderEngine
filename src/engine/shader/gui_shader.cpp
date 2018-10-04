#include "gui_shader.hpp"

GuiShader::GuiShader() :
	Shader(VERTEX_FILE, FRAGMENT_FILE)
{
	this->start();
	loc_projMat = getUniformLoc("projection_mat");
	loadInt("tex", 0);
	this->stop();
}