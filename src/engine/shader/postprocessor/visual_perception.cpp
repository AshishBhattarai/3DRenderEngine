#include "visual_perception.hpp"

VisualPerception::VisualPerception():
	Shader(VERTEX_FILE, FRAGMENT_FILE)
{
	loc_brightness = getUniformLoc("brightness");
	loc_contrast = getUniformLoc("contrast");
	loc_gamma = getUniformLoc("gamma");

	// texture unit for sampler
	start();
	loadInt("frame_texture", 0);
	stop();
}