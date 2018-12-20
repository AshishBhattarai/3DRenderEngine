#ifndef SHAPE_MESH_HPP
#define SHAPE_MESH_HPP

// Mesh whose vertices are manually defined/hardcoded

class ShapeMesh {
private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	int count;

public:
	ShapeMesh(float* data, int dim/*dimension*/, int numVertex, bool normal, bool texCoords);
	~ShapeMesh();

	void setIndices(unsigned int* data, int numIndices);

	void draw();

	unsigned int getVAO() const {
		return VAO;
	}
	int getCount() const {
		return count;
	}
};

#endif