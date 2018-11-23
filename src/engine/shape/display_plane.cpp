#include "display_plane.hpp"

DisplayPlane::DisplayPlane() {
	float planeVertices[] = {
    // positions   // texCoords
   	-1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
  	1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
	};
	mesh = std::make_unique<ShapeMesh>(planeVertices, 2, 6, false, true);
}