#ifndef MTL_PARSER_HPP
#define MTL_PARSER_HPP

#include <vector>
#include <string>
#include <string_view>
#include <glm/glm.hpp>


class MtlParser {
public:
struct Material {
	std::string name;

	// colors
	glm::vec3 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	glm::vec3 Ke;

	// factors
	float Ns;
	float Ni;
	float d;
	int illum;

	// texture maps
	std::string map_Ka;
	std::string map_Kd;
	std::string map_Ks;
	std::string map_Ke;

	std::string map_Ns;
	std::string map_d;
	std::string map_bump;

	Material():
		Ka(0.0f),
		Kd(0.0f),
		Ks(0.0f),
		Ke(0.0f),
		Ns(0.0f),
		Ni(0.0f),
		d(0.0f),
		illum(0)
	{}
};

private:
	// consts
	static constexpr std::string_view COMMENT						=		"#";
	static constexpr std::string_view NEW_MATERIAL			= 	"newmtl";
	static constexpr std::string_view COLOR_AMBIENT 		= 	"Ka";
	static constexpr std::string_view COLOR_DIFFUSE			=		"Kd";
	static constexpr std::string_view COLOR_SPECULAR		=		"Ks";
	static constexpr std::string_view COLOR_EMISSION		=		"Ke";
	static constexpr std::string_view SHININESS					=		"Ns";
	static constexpr std::string_view OPTICAL_DENSITY		=		"Ni";
	static constexpr std::string_view DISSOLVE					=		"d";
	static constexpr std::string_view ILLUMINATION			=		"illum";
	static constexpr std::string_view MAP_AMBIENT				=		"map_Ka";
	static constexpr std::string_view MAP_DIFFUSE				=		"map_Kd";
	static constexpr std::string_view MAP_SPECULAR			=		"map_Ks";
	static constexpr std::string_view MAP_EMISSION			=		"map_Ke";
	static constexpr std::string_view MAP_SHININESS			= 	"map_Ns";
	static constexpr std::string_view MAP_DISSOLVE			=	 	"map_d";
	static constexpr std::string_view MAP_BUMP					=		"map_bump";

	std::vector<Material> materials;

	std::vector<std::string> splitLine(std::string& line);
	bool parseMaterials(const std::string& path);

public:
	MtlParser(const std::string& path);

	int getNumMaterials() const {
		return materials.size();
	}

	std::vector<Material> getMaterials() const {
		return materials;
	}

	Material getMaterial(int index = 0) const {
		return materials[index];
	}
};

#endif