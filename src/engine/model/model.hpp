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
		BASIC 		= 	1,
		MATERIAL 	= 	2,
		TEXTURED 	=	 	3
	};

private:
	std::vector<std::unique_ptr<Mesh>> meshes; // meshs of the model;
	std::string directory; // absolute path of the model file
	// map of path(local+filename) and loaded textured
	// cleared after loading
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures_loaded;

	Type modelType;
	int flags;
	int numMeshes; // no of meshes in the model;

	// load the model file
	void loadModel(std::string_view path);
	// conver aiMesh(assimp) to Mesh
	std::unique_ptr<Mesh> processMesh(const aiMesh* mesh, const aiScene* scene);
	// load the material colors
	MaterialMesh::Material loadMaterialColor(const aiMaterial* mat);
	// load material textures
	std::shared_ptr<Texture> loadMaterialTexture(const aiMaterial* mat, std::string path, Texture::Type type);

public:
	Model(std::string_view path, Type type, int flags = 0);

	int getnumMeshes() const {
		return numMeshes;
	}

	Type getModelType() const {
		return modelType;
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