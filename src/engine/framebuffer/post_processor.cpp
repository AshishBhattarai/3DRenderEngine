#include "post_processor.hpp"

#include <glad/glad.h>
#include "shape/display_plane.hpp"
#include "renderEngine/display_manager.hpp"

PostProcessor::PostProcessor() :
	gamma(1.0f),
	brightness(0.0f),
	contrast(0.0f),
	paint_radius(8),
	vpShader(),
	peShader(),
	disPlane(DisplayPlane::getInstance())
{
	vpShader.start();
	vpShader.loadGamma(gamma);
	vpShader.loadBrightness(brightness);
	vpShader.loadContrast(contrast);
	vpShader.stop();

	float width = DisplayManager::getInstance().getScreenWidth();
	float height = DisplayManager::getInstance().getScreenHeight();
	peShader.start();
	peShader.loadRadius(paint_radius);
	peShader.loadScreenSize(glm::vec2(width, height));
	peShader.stop();
}

void PostProcessor::applyVisualPerception(unsigned int textureID) {
	vpShader.start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glBindVertexArray(disPlane.getMesh()->getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_FRAMEBUFFER_SRGB);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	vpShader.stop();
}

void PostProcessor::applyPaintEffect(unsigned int textureID) {
	peShader.start();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(disPlane.getMesh()->getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	peShader.stop();
}

void PostProcessor::setGamma(float gamma) {
	this->gamma = gamma;
	vpShader.start();
	vpShader.loadGamma(gamma);
	vpShader.stop();
}

void PostProcessor::setBrightness(float brightness) {
	this->brightness = brightness;
	vpShader.start();
	vpShader.loadBrightness(brightness);
	vpShader.stop();
}

void PostProcessor::setContrast(float contrast) {
	this->contrast = contrast;
	vpShader.start();
	vpShader.loadContrast(contrast);
	vpShader.stop();
}