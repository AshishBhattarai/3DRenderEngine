#include "paint_effect.hpp"

PaintEffect::PaintEffect() :
	Shader(VERTEX_FILE, FRAGMENT_FILE)
{
	loc_radius = getUniformLoc("radius");
	loc_scrSize = getUniformLoc("scr_size");

	start();
	loadInt("tex", 0);
	stop();
}