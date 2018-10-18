#ifndef UNIFORM_BLOCKS_HPP
#define UNIFORM_BLOCKS_HPP

// This namespace contains names and constants defined in the shader

namespace ShaderConfig {

	// Matrices + Camera position ie global vertex shader data
	constexpr const char* GeneralVSData			 = "GeneralVSData";
	constexpr const char* GeneralFSData			 = "GeneralFSData";

	constexpr int MAX_POINT_LIGHTS = 10;
	constexpr int MAX_SPOT_LIGHTS  = 10;
};

#endif