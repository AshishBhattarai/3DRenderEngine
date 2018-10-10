#include "model.hpp"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "utils/slogger.hpp"

void Model::loadModel(std::string_view path) {
	Assimp::Importer importer;

	// load model into assimp scene
	const aiScene* scene = importer.ReadFile(
		std::string(path),
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_ImproveCacheLocality |
		aiProcess_LimitBoneWeights |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_FindInvalidData |
		aiProcess_GenSmoothNormals |
		aiProcess_OptimizeMeshes |
		aiProcess_OptimizeGraph |
		aiProcess_JoinIdenticalVertices
	);
	//error check
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		SLOG("ERROR: ", importer.GetErrorString());
		return;
	}
	// if the meshes aren't triangulated
	if(scene->mMeshes[0]->mFaces[0].mNumIndices > 3)
		importer.ApplyPostProcessing(aiProcess_Triangulate);
	//  if type is set to textured but model doesn't provide texture coords
	if(!scene->mMeshes[0]->HasTextureCoords(0) && (modelType == MATERIAL_TEXTURE)) {
		CSLOG("ERROR: Model type TEXTURED but the file  doesn't contain",
					"textureCoords ", directory);
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	// process all the meshes in the scene
	for(unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		meshes.emplace_back(processMesh(scene->mMeshes[i], scene));
	}
	numMeshes = meshes.size();
}

std::unique_ptr<Mesh> Model::processMesh(const aiMesh* mesh, const aiScene* scene) {
	// mesh attributes
	std::vector<Mesh::Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<glm::vec2> texCoords;
	MaterialMesh::Material color;
	Texture::Map textures;
	std::string name(mesh->mName.C_Str());
	bool hasTexture = (modelType == MATERIAL_TEXTURE);
	// for textured mesh
	float specular_shininess = MaterialMesh::default_shininess;

	// reserve space
	vertices.reserve(mesh->mNumVertices);
	indices.reserve(mesh->mNumFaces*3);
	if(hasTexture) texCoords.reserve(mesh->mNumVertices);

	// load vertices and textureCoords
	for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Mesh::Vertex vertex;
		glm::vec2 texCoord;

		// position
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		calculatebb(vertex.position);

		// normals
		if(flags & ModelFlags::FAKE_NORMAL) {
			vertex.normal = glm::vec3(0.0f, 1.0f, 0.0);
		} else {
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		vertices.emplace_back(std::move(vertex));

		if(hasTexture) {
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			texCoords.emplace_back(std::move(texCoord));
		}
	}

	// load indices
	for(unsigned int i = 0; i < mesh->mNumFaces; ++i) { // get faces
		aiFace face = mesh->mFaces[i];
		for(GLuint j = 0; j < face.mNumIndices; ++j) // get indices
			indices.emplace_back(face.mIndices[j]);
	}

	// process material color & texture
	const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

	if(modelType == MATERIAL_COLOR) {
		loadMaterialColor(mat, color);
	} else {
		loadMaterialTexture(mat, textures);
		mat->Get(AI_MATKEY_SHININESS, specular_shininess);
	}

	if(hasTexture)
		return std::make_unique<TexturedMesh>(vertices, indices, texCoords,
			std::move(textures), specular_shininess, name);
	else if(modelType == MATERIAL_COLOR)
		return std::make_unique<MaterialMesh>(vertices, indices, color, name);
	else
		return std::make_unique<Mesh>(vertices, indices, name);
}

void Model::loadMaterialColor(const aiMaterial* mat, MaterialMesh::Material& color) {
	// error message
	auto message = [directory = directory](const std::string& type) {
		SLOG("Failed to load " + type + " color for ",
			directory, ", Using default value.");
	};

	// load material rest colors
	aiColor3D ambient(MaterialMesh::default_ambient);
	if(aiReturn_SUCCESS != mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient))
		message("ambient");

	aiColor3D diffuse(MaterialMesh::default_diffuse);
	if(aiReturn_SUCCESS != mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
		message("diffuse");

	// load specular factor ans shininess
	aiColor3D specular(MaterialMesh::default_specular);
	if(mat->Get(AI_MATKEY_COLOR_SPECULAR, specular))
		message("specular");

	float shininess(MaterialMesh::default_shininess);
	if(aiReturn_SUCCESS != mat->Get(AI_MATKEY_SHININESS, shininess))
		message("shininess");

	// name
	aiString name;
	mat->Get(AI_MATKEY_NAME, name);

	// createing MaterialMesh::Material
	color.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
	color.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
	color.specular = glm::vec3(specular.r, specular.g, specular.b);
	color.shininess = shininess;
	color.name = name.C_Str();
}

void Model::loadMaterialTexture(const aiMaterial* mat, Texture::Map& textures) {
	// load textures if they exist otherwise set 0
	aiString path;
	if(mat->GetTextureCount(aiTextureType_DIFFUSE)) {
		// diffuse
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		textures[Texture::DIFFUSE_MAP] = loadTexture(path.C_Str(), Texture::DIFFUSE_MAP);
	}
	if(mat->GetTextureCount(aiTextureType_SPECULAR)) {
		//specular
		mat->GetTexture(aiTextureType_SPECULAR, 0, &path);
		textures[Texture::SPECULAR_MAP] = loadTexture(path.C_Str(), Texture::SPECULAR_MAP);
	}
}

std::shared_ptr<Texture> Model::loadTexture(std::string path, Texture::Type type) {
	// absolute path
	path = directory + '/' + path;

	// check if texture is already loaded
	if(textures_loaded.find(path) == textures_loaded.end())
		textures_loaded[path] = std::make_shared<Texture>(path, type); // add to map
	// loaded
	return textures_loaded[path]; // return shared pointer
}

void Model::calculatebb(glm::vec3& position) {
	if(minbb.x > position.x)
		minbb.x = position.x;
	else if(maxbb.x < position.x)
		maxbb.x = position.x;

	if(minbb.y > position.y)
		minbb.y = position.y;
	else if(maxbb.y < position.y)
		maxbb.y = position.y;

	if(minbb.z > position.z)
		minbb.z = position.z;
	else if(maxbb.z < position.z)
		maxbb.z = position.z;
}

Model::Model(std::string_view path, Type type, int flags):
		modelType(type),
		flags(flags),
		numMeshes(0),
		minbb(glm::vec3(0.0f)),
		maxbb(glm::vec3(0.0f))
{
	loadModel(path);
}