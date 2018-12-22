#ifndef MATERIAL_MESH_HPP
#define MATERIAL_MESH_HPP

#include "mesh.hpp"

// this clas represents a model of the mesh with material
class MaterialMesh : public Mesh {
public:
	// default colors - white;
	static constexpr float default_diffuse = 0.5f;
	static constexpr float default_specular = 1.0f;
	static constexpr float default_shininess = 1.0f;

	// color of the mesh
	struct Material {
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		std::string name; // material name
	};

protected:
	static constexpr float REMOVE_GAMMA = 2.2f;
	// mesh data
	Material material;

	MaterialMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices,
		std::string name = "") :
		Mesh(vertices, indices, name)
	{}

	void removeGamma();

public:
	MaterialMesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices,
			Material material, std::string name = "") :
			Mesh(vertices, indices, name),
			material(material)
	{}

	Material getMaterial() const {
		return material;
	}

	void setMaterial(Material material);
};

#endif