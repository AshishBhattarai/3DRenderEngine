#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <memory>
#include <unordered_map>

// This class represents a texture and its properties

class Texture {
public:
	// type of textures
	enum Type {
		// model textures
		DIFFUSE_MAP				=			000,
		SPECULAR_MAP			=			001,

		// terrian textures(splat map)
		BLEND_MAP					=			100,
		RED_TEXTURE				=			101,
		GREEN_TEXTURE			=			102,
		BLUE_TEXTURE			=			103,
		BLACK_TEXTURE			=			104
	};

	// texture map
	using Map = std::unordered_map<Type, std::shared_ptr<Texture>>;

	// TODO: Test these values properly
	// LOD BIAS (gets added to the textures lod) -ve high, +ve low
	static constexpr float HIGH_LOD 	=		-0.8f;
	static constexpr float MID_LOD  	= 	-0.4f;
	static constexpr float LOW_LOD		=		 0.0f;

	// wraps
	static constexpr u_int CLAMP 	=	GL_CLAMP_TO_EDGE;
	static constexpr u_int REPEAT	=	GL_REPEAT;

private:
	// members
	GLuint id;
	Type type;
	float lod_bias;

	void loadTexture(const std::string& path, bool flip_y);

public:
	Texture() : id(0) {}
	Texture(const std::string& path, Type type = DIFFUSE_MAP, bool flip_y = false, float lod_bias = MID_LOD);
	~Texture();

	// setters
	void setLOD(float lod_bias);

	// set wrap
	void setTextureWrap(u_int wrap);

	// getters
	GLuint getID() const {
		return id;
	}

	Type getType() const {
		return type;
	}

	float getLOD() const {
		return lod_bias;
	}

};
#endif

