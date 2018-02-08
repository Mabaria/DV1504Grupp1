#include "MaterialHandler.h"

MaterialHandler::MaterialHandler(const aiScene * scene)
{
	this->mpScene = scene;
	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		this->mMaterialStructVector.push_back(
			this->mCreateMaterialStruct(i));
		// Populate vector with ready made structs
	}
}

MaterialHandler::~MaterialHandler()
{
}

std::string MaterialHandler::GetMaterialName(const unsigned int materialIndex) const
{
	aiString name;
	if (this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_NAME, name)
		!= AI_SUCCESS)
	{
		throw "Failed to get name component of material.";
	}
	return std::string(name.data);
}

aiColor3D MaterialHandler::GetDiffuseColor(const unsigned int materialIndex) const
{
	aiColor3D color(0.f, 0.f, 0.f);
	if (this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, color)
		!= AI_SUCCESS)
	{
		throw "Failed to get diffuse component of material.";
	}
	return color;
}

aiColor3D MaterialHandler::GetAmbientColor(const unsigned int materialIndex) const
{
	aiColor3D color(0.f, 0.f, 0.f);
	if (this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, color)
		!= AI_SUCCESS)
	{
		throw "Failed to get ambient component of material.";
	}
	return color;
}

aiColor3D MaterialHandler::GetSpecularColor(const unsigned int materialIndex) const
{
	aiColor3D color(0.f, 0.f, 0.f);
	if (this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, color)
		!= AI_SUCCESS)
	{
		throw "Failed to get specular component of material.";
	}
	return color;
}

aiColor3D MaterialHandler::GetTransparentColor(const unsigned int materialIndex) const
{
	aiColor3D color(0.f, 0.f, 0.f);
	if (this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_TRANSPARENT, color)
		!= AI_SUCCESS)
	{
		throw "Failed to get transparent component of material.";
	}
	return color;
}

aiBlendMode MaterialHandler::GetBlendMode(const unsigned int materialIndex) const
{
	aiBlendMode blend_mode = aiBlendMode_Default;
	if (this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_TRANSPARENT, blend_mode)
		!= AI_SUCCESS)
	{
		throw "Failed to get blend mode component of material.";
	}
	return blend_mode;
}

float MaterialHandler::GetShininess(const unsigned int materialIndex) const
{
	float shininess = 0.f;
	if (this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_SHININESS, shininess)
		!= AI_SUCCESS)
	{
		throw "Failed to get shininess component of material.";
	}
	return shininess;
}

float MaterialHandler::GetShininessStrength(const unsigned int materialIndex) const
{
	float strength = 1.0f;
	if (this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_SHININESS_STRENGTH, strength)
		!= AI_SUCCESS)
	{
		throw "Failed to get shininess strength component of material.";
	}
	return strength;
}

std::string MaterialHandler::GetDiffuseTexturePath(const unsigned int materialIndex) const
{
	aiString texture_path;
	if (this->mpScene->mMaterials[materialIndex]->Get(
		AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_path)
		!= AI_SUCCESS)
	{
		throw "Failed to get diffuse texture path component of material.";
	}
	return std::string(texture_path.data);
}

aiTextureMapMode MaterialHandler::GetMappingModeU(const unsigned int materialIndex) const
{
	aiTextureMapMode mapping_mode = aiTextureMapMode_Wrap;
	if (this->mpScene->mMaterials[materialIndex]->Get(
		AI_MATKEY_MAPPINGMODE_U(aiTextureType_DIFFUSE, 0), mapping_mode)
		!= AI_SUCCESS)
	{
		throw "Failed to get Mapping Mode U component of material.";
	}
	return mapping_mode;
}

aiTextureMapMode MaterialHandler::GetMappingModeV(const unsigned int materialIndex) const
{
	aiTextureMapMode mapping_mode = aiTextureMapMode_Wrap;
	if (this->mpScene->mMaterials[materialIndex]->Get(
		AI_MATKEY_MAPPINGMODE_V(aiTextureType_DIFFUSE, 0), mapping_mode)
		!= AI_SUCCESS)
	{
		throw "Failed to get Mapping Mode V component of material.";
	}
	return mapping_mode;
}

MaterialStruct MaterialHandler::GetMaterialStruct(const unsigned int materialIndex) const
{
	if (materialIndex < this->mMaterialStructVector.size())
	{
		return this->mMaterialStructVector[materialIndex];
	}
}

MaterialStruct MaterialHandler::mCreateMaterialStruct(const unsigned int materialIndex)
{
	MaterialStruct to_return;
	aiColor3D diffuseC = this->GetDiffuseColor(materialIndex);
	aiColor3D specularC = this->GetSpecularColor(materialIndex);
	aiColor3D ambientC = this->GetAmbientColor(materialIndex);
	float exp = this->GetShininess(materialIndex);
	to_return =
	{
		diffuseC.r, diffuseC.g, diffuseC.b,
		ambientC.r, ambientC.g, ambientC.b,
		specularC.r, specularC.g, specularC.b,
		exp
	};
	return to_return;
}