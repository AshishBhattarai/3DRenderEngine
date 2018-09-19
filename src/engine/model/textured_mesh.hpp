#ifndef TEXTURED_MESH_HPP
#define TEXTURED_MESH_HPP

#include "mesh.hpp"

#include <unordered_map>
#include "texture.hpp"

// This class repersents a mesh with texture

class TexturedMesh : public Mesh {
private:
	// textures baised on type
	Texture::Map textures;
	float specular_shininess;

	// texture coords
	GLuint t_VBO;

	// load texture coords
	void setupTextureCoords(std::vector<glm::vec2>& texCoords);

public:
	// only texCoords
	TexturedMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices,
		std::vector<glm::vec2>& texCoords, float shininess = 1, std::string name = "");

	// single texture
	TexturedMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices,
		std::vector<glm::vec2>& texCoords, std::shared_ptr<Texture> tex_diffuse,
		float shininess = 1, std::string name = "");

	// texCoords + texture map
	TexturedMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices,
		std::vector<glm::vec2>& texCoords, Texture::Map&& textures,
		float shininess = 1, std::string name = "");

	~TexturedMesh();

	const Texture* getTexture(const Texture::Type& type) const {
		auto tex = textures.find(type);
		return (tex == textures.end())? nullptr : tex->second.get();
	}

	// get id if texture exists else 0
	GLuint getTextureID(const Texture::Type& type) const {
		auto tex = textures.find(type);
		return (tex == textures.end())? 0 : tex->second->getID();
	}

	// Note: this overrides last texture(if there was any)
	void addTexture(std::shared_ptr<Texture> texture) {
		textures[texture->getType()] = texture;
	}

	void setTextureWrapAll(u_int wrap) {
		for(auto pair : textures) {
			pair.second->setTextureWrap(wrap);
		}
	}

	void setTextureLODAll(float lod) {
		for(auto pair : textures) {
			pair.second->setLOD(lod);
		}
	}

	void setTextureMap(const Texture::Map& map) {
		textures = map;
	}

	void setTextureMap(Texture::Map&& map) {
		textures = map;
	}

	void setSpecularShininess(float shininess) {
		specular_shininess = shininess;
	}

	bool hasTexture(Texture::Type type) const {
		return getTextureID(type);
	}

	float getSpecularShininess() const {
		return specular_shininess;
	}
};

#endif