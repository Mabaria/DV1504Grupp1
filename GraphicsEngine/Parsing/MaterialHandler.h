#pragma once
#include "Mesh.h"
class MaterialHandler
{
public:
	MaterialHandler(const aiScene *scene);

	~MaterialHandler();


	aiColor3D GetDiffuseColor() const;
	aiColor3D GetAmbientColor() const;
	aiColor3D GetSpecularColor() const;
	float GetShininess() const;
	float GetOpacity();



private:
	const aiScene* mScene;

};