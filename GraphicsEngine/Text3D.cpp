#include "Text3D.h"

Text3D::Text3D(
	std::string name,
	const std::vector<std::vector<unsigned int>> indices,
	const std::vector<std::vector<Vertex>> vertices,
	const std::wstring text_path,
	D3D11& d3d11)
{
	this->s = nullptr;
}

Text3D::~Text3D()
{
}
