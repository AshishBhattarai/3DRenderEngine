#include "mtl_parser.hpp"

#include "slogger.hpp"
#include <fstream>
#include <iterator>
#include <sstream>

std::vector<std::string> MtlParser::splitLine(std::string& line) {
	// split line on 'space'
	std::stringstream sLine(line);
	std::istream_iterator<std::string> begin(sLine);
	std::istream_iterator<std::string> end;
	std::vector<std::string> split(begin, end);

	return split;
}

bool MtlParser::parseMaterials(const std::string& path) {
	// check extension
	if(path.substr(path.size()-4) != ".mtl")
		return false;

	std::ifstream file(path);
	if(!file.is_open())
		return false;

	Material mat;
	std::vector<std::string> split;
	std::string line;

	while(std::getline(file, line)) {
		if(line.empty()) continue;
		// split the line
		split = splitLine(line);

		if(split[0] == COMMENT) {
			continue;
		}

		if(split[0] == NEW_MATERIAL) {
			// push last material and reset mat for new
			if(!mat.name.empty()) {
				materials.push_back(mat);
				mat = Material();
			}
			mat.name = split[1];
		} else {
			if(split[0] == COLOR_AMBIENT) {
				mat.Ka.x = std::stof(split[1]);
				mat.Ka.y = std::stof(split[2]);
				mat.Ka.z = std::stof(split[3]);
			} else if(split[0] == COLOR_DIFFUSE) {
				mat.Kd.x = std::stof(split[1]);
				mat.Kd.y = std::stof(split[2]);
				mat.Kd.z = std::stof(split[3]);
			} else if(split[0] == COLOR_SPECULAR) {
				mat.Ks.x = std::stof(split[1]);
				mat.Ks.y = std::stof(split[2]);
				mat.Ks.z = std::stof(split[3]);
			} else if(split[0] == COLOR_EMISSION) {
				mat.Ke.x = std::stof(split[1]);
				mat.Ke.y = std::stof(split[2]);
				mat.Ke.z = std::stof(split[3]);
			} else if(split[0] == SHININESS) {
				mat.Ns = std::stof(split[1]);
			} else if(split[0] == OPTICAL_DENSITY) {
				mat.Ni = std::stof(split[1]);
			} else if(split[0] == DISSOLVE) {
				mat.d = std::stof(split[1]);
			} else if(split[0] == ILLUMINATION) {
				mat.illum = std::stoi(split[1]);
			} else if(split[0] == MAP_AMBIENT) {
				mat.map_Ka = split[1];
			} else if(split[0] == MAP_DIFFUSE) {
				mat.map_Kd = split[1];
			} else if(split[0] == MAP_SPECULAR) {
				mat.map_Ks = split[1];
			} else if(split[0] == MAP_EMISSION) {
				mat.map_Ke = split[1];
			} else if(split[0] == MAP_SHININESS) {
				mat.map_Ks = split[1];
			} else if(split[0] == MAP_DISSOLVE) {
				mat.map_d = split[1];
			} else if(split[0] == MAP_BUMP) {
				mat.map_bump = split[1];
			}
		}
	}
	// add last material
	materials.push_back(mat);

	if(materials.empty())
		return false;

	return true;
}

MtlParser::MtlParser(const std::string& path) {
	if(!parseMaterials(path))
		CSLOG("Failed to parser, empty or invalid file.");
}