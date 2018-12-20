#ifndef POST_PROCESSOR_HPP
#define POST_PROCESSOR_HPP

#include "framebuffer.hpp"
#include "shader/postprocessor/visual_perception.hpp"
#include "shader/postprocessor/paint_effect.hpp"

class DisplayPlane;
class PostProcessor {
private:

	float gamma;
	float brightness;
	float contrast;
	int paint_radius;

	VisualPerception vpShader;
	PaintEffect peShader;

	DisplayPlane& disPlane;

public:
	 PostProcessor();

	/**
	 * VisualPerception is brightness + contrast + gamma
	 * 
	 */
	void applyVisualPerception(unsigned int textureID);
	void applyPaintEffect(unsigned int textureID);

	// setter and getters
	void setGamma(float gamma);
	void setBrightness(float brightness);
	void setContrast(float contrast);
	void setPaintRadius(int radius);

	float getGamma() const {
		return gamma;
	}

	float getBrightness() const {
		return brightness;
	}

	float getContrast() const {
		return contrast;
	}

	int getPaintRadius() const {
		return paint_radius;
	}
};

#endif