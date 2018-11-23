#ifndef DISPLAY_PLANE_HPP
#define DISPLAY_PLANE_HPP

#include <memory>

#include "shape_mesh.hpp"

// plane which covers the display window

class DisplayPlane {
private:
	std::unique_ptr<ShapeMesh> mesh;

	DisplayPlane();

public:
	// delete copy
	DisplayPlane(const DisplayPlane&) = delete;
	DisplayPlane& operator=(const DisplayPlane&) = delete;

	// instance
	static DisplayPlane& getInstance() {
		static DisplayPlane instance;
		return instance;
	}

	ShapeMesh* getMesh() const {
		return mesh.get();
	}
};

#endif