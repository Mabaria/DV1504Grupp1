#pragma once

#include "Parsing/Mesh.h"
#include <vector>

class Quad
{
public:
	Quad(const bool used_for_textures);

	~Quad();

	std::vector<std::vector<Vertex>> GetVertices();
	std::vector<std::vector<unsigned int>> GetIndices();

private:
	std::vector<std::vector<Vertex>> mVertices;
	std::vector<std::vector<unsigned int>> mIndices;
};