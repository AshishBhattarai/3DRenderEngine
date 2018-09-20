#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "shader/shader.hpp"

// represent a mesh of the model without texture or material

class Mesh {
public:
	// verterx data
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
	};

private:
	std::string name; // name of the mesh
	int indicesCount; // no. of indices to render

	GLuint v_VBO, EBO;

	// load vertices
	void setupMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices);

protected:
	// vertex data
	GLuint VAO;
	int attributesCount; // no. of vertex attributes

public:
	Mesh(std::vector<Vertex>& vertices, std::vector<uint>& indices,
		std::string name = "");
	// move
	Mesh(Mesh&& mesh);
	// copy
	Mesh(Mesh& mesh);

	virtual ~Mesh();

	int incAttributesCount() {
		return 	attributesCount++;
	}

	const GLuint getVAO() const {
		return VAO;
	}

	int getAttributesCount() const {
		return 	attributesCount;
	}

	int getIndicesCount() const {
		return indicesCount;
	}

	std::string getName() const {
		return name;
	}
};

#endif