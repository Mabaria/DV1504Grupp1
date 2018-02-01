#pragma once

#include "../UI/UIElements/MeshObject.h"
#include "DX/Direct3D.h"

class Text3D
{
public:
	Text3D(
		std::string name, 
		const std::vector<std::vector<unsigned int>> indices,
		const std::vector<std::vector<Vertex>> vertices,
		const std::wstring text_path,
		D3D11& d3d11
	);
	~Text3D();

private:

	ID3D11ShaderResourceView * s;


};