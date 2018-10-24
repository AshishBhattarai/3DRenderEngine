#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <memory>
#include <unordered_map>
#include <vector>

// This class represents a texture and its properties

class Texture {
public:
	// type of textures
	enum Type {
		// model textures
		DIFFUSE_MAP				=			000,
		SPECULAR_MAP			=			001,
		EMISSION_MAP			=			003,

		// terrian textures(splat map)
		BLEND_MAP							=			100,
		DIFFUSE_RED_MAP				=			101,
		DIFFUSE_GREEN_MAP			=			102,
		DIFFUSE_BLUE_MAP			=			103,
		DIFFUSE_BLACK_MAP			=			104,

		DIFFUSE_CUBE_MAP							=			500
	};

	// texture map
	using Map = std::unordered_map<Type, std::shared_ptr<Texture>>;

	// TODO: Test these values properly
	// LOD BIAS (gets added to the textures lod) -ve high, +ve low
	static constexpr float HIGH_LOD 	=		-0.8f;
	static constexpr float MID_LOD  	= 	-0.4f;
	static constexpr float LOW_LOD		=		 0.0f;

	// wraps
	static constexpr GLenum CLAMP 	=	GL_CLAMP_TO_EDGE;
	static constexpr GLenum REPEAT	=	GL_REPEAT;

	static constexpr int CUBEMAP_NUM_FACES = 6; // cubemap faces

private:
	// members
	GLuint id;
	Type type;
	float lod_bias;

	bool converSRGB();
	void loadTexture(const std::string& path, bool flip_y);
	void loadCubeMap(const std::array<std::string, CUBEMAP_NUM_FACES>& paths, bool flip_y);

public:
	Texture() : id(0) {}
	Texture(const std::string& path, Type type = DIFFUSE_MAP, bool flip_y = false, float lod_bias = MID_LOD);
	// cubemap
	Texture(const std::array<std::string, CUBEMAP_NUM_FACES>& paths, bool flip_y = false);
	~Texture();

	// setters
	void setLOD(float lod_bias);

	// set wrap
	void setTextureWrap(GLenum wrap);

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

