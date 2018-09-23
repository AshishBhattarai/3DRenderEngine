#ifndef MODEL_HPP
#define MODEL_HPP

#include <assimp/scene.h>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "material_mesh.hpp"
#include "textured_mesh.hpp"

// This class represents a 3D model which is made up of multiple meshes

namespace ModelFlags {
	constexpr int FAKE_NORMAL = 0x1;
	constexpr int	TRANSPARENT = 0x2;
};

class Model {
public:
	// type of model
	enum Type {
		BASIC 						= 	1,
		MATERIAL_COLOR 		= 	2,
		MATERIAL_TEXTURE 	=	 	3
	};

private:
	std::vector<std::unique_ptr<Mesh>> meshes; // meshs of the model;
	std::string directory; // absolute path of the model file
	// map of path(local+filename) and loaded textured
	// cleared after loading
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures_loaded;

	Type modelType;
	int flags;
	u_int numMeshes; // no of meshes in the model;

	glm::vec3 minbb; // min bounding box
	glm::vec3 maxbb; // max bounding box

	// load the model file
	void loadModel(std::string_view path);
	// conver aiMesh(assimp) to Mesh
	std::unique_ptr<Mesh> processMesh(const aiMesh* mesh, const aiScene* scene);
	// load the material colors
	void loadMaterialColor(const aiMaterial* mat, MaterialMesh::Material& color);
	// loads all the textures
	void loadMaterialTexture(const aiMaterial* mat, Texture::Map& textures);
	// load texture data
	std::shared_ptr<Texture> loadTexture(std::string path, Texture::Type type);
	// calculate min&maxbb
	void calculatebb(glm::vec3& position);

public:
	Model(std::string_view path, Type type, int flags = 0);

	u_int getnumMeshes() const {
		return numMeshes;
	}

	Type getModelType() const {
		return modelType;
	}

	glm::vec3 getMinbb() const {
		return minbb;
	}

	glm::vec3 getMaxbb() const {
		return maxbb;
	}

	const TexturedMesh* getTexturedMesh(int i = 0) const {
		return dynamic_cast<const TexturedMesh*>(meshes[i].get());
	}

	const MaterialMesh* getMaterialMesh(int i = 0) const {
		return dynamic_cast<const MaterialMesh*>(meshes[i].get());
	}

	// get raw mesh maybe to make a copy or idk
	// non const so that it can be modified
	Mesh* getRawMesh(int i = 0) const {
		return meshes[i].get();
	}
};

#endif