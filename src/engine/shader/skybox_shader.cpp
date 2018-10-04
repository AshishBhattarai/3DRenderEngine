#include "skybox_shader.hpp"

#include "renderEngine/display_manager.hpp"

SkyboxShader::SkyboxShader():
	Shader(VERTEX_FILE, FRAGMENT_FILE)
{
	this->start();
	loadInt("skyboxTexture", 0);
	loc_skyboxMat = getUniformLoc("skybox_mat");
	this->stop();
}

void SkyboxShader::loadSkyboxMatrix(const glm::mat4& view, float rmp) {
	// view without translation
	glm::mat4 skyboxMat = glm::mat3(view);
	// roatate
	if(rmp != 0.0f) {
		float rot = rmp * (DisplayManager::getInstance().getTimeSec()/60000.0f);
		skyboxMat  = glm::rotate(skyboxMat, rot, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	uniformMatrix4fv(loc_skyboxMat, skyboxMat);
}