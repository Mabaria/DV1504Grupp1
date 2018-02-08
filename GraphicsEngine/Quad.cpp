#include "Quad.h"

Quad::Quad(const bool use_with_texture)
{
	// ___ VERTICES ___

	std::vector<Vertex> vertices;

	Vertex v0 = {
		0.0f, 0.0f,  0.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, -1.0f
	};

	Vertex v1 = {
		0.0f, 1.0f,  0.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, -1.0f
	};

	Vertex v2 = {
		1.0f, 0.0f,  0.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, -1.0f
	};

	Vertex v3 = {
		1.0f, 1.0f,  0.0f,
		0.0f, 0.0f, -1.0f,
		-1.0f, -1.0f
	};

	if (use_with_texture)
	{
		v0.texU = 0.0f;
		v0.texV = 1.0f;

		v1.texU = 0.0f;
		v1.texV = 0.0f;

		v2.texU = 1.0f;
		v2.texV = 1.0f;

		v3.texU = 1.0f;
		v3.texV = 0.0f;
	}

	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	this->mVertices.push_back(vertices);


	// ___ INDICES ___

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);

	this->mIndices.push_back(indices);

}

Quad::~Quad()
{
}

std::vector<std::vector<Vertex>> Quad::GetVertices()
{
	return this->mVertices;
}

std::vector<std::vector<unsigned int>> Quad::GetIndices()
{
	return this->mIndices;
}
