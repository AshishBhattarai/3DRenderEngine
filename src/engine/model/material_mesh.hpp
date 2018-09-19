#ifndef MATERIAL_MESH_HPP
#define MATERIAL_MESH_HPP

#include "mesh.hpp"

// this clas represents a model of the mesh with material
class MaterialMesh : public Mesh {
public:
	// default colors - white;
	static constexpr float default_ambient = 0.1f;
	static constexpr float default_diffuse = 0.5f;
	static constexpr float default_specular = 1.0f;
	static constexpr float default_shininess = 16.0f;

	// color of the mesh
	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		std::string name; // material name
	};

private:
	// mesh data
	Material color;

public:
	MaterialMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices,
			Material color, std::string name = "") :
			Mesh(vertices, indices, name),
			color(color)
	{}

	const Material& getMaterialColor() {
		return color;
	}
};

#endif