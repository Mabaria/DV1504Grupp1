#include "MaterialHandler.h"

MaterialHandler::MaterialHandler(const aiScene * scene)
{
	this->mpScene = scene;
	if (scene != nullptr)
	{
		this->mNumberOfMaterials = scene->mNumMaterials;
		for (unsigned int i = 0; i < this->mNumberOfMaterials; i++)
		{
			this->mMaterialStructVector.push_back(
				this->mCreateMaterialStruct(i));
			// Populate vector with ready made structs
		}
	}
	else
	{
		this->mNumberOfMaterials = 0;
	}
}

MaterialHandler::~MaterialHandler()
{
}

void MaterialHandler::SetMaterialStruct(const MaterialStruct & newStruct, const unsigned int materialIndex)
{
	if (materialIndex >= this->mNumberOfMaterials)
	{
		this->mMaterialStructVector.push_back(newStruct); // Add new material
	}
	else
	{
		this->mMaterialStructVector[materialIndex] = newStruct; // Replace old material
	}
}

std::string MaterialHandler::GetMaterialName(const unsigned int materialIndex) const
{
	aiString name;
	this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_NAME, name);
	return std::string(name.data);
}

aiColor3D MaterialHandler::GetDiffuseColor(const unsigned int materialIndex) const
{
	aiColor3D color(0.f, 0.f, 0.f);
	this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	return color;
}

aiColor3D MaterialHandler::GetAmbientColor(const unsigned int materialIndex) const
{
	aiColor3D color(0.f, 0.f, 0.f);
	this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_AMBIENT, color);
	return color;
}

aiColor3D MaterialHandler::GetSpecularColor(const unsigned int materialIndex) const
{
	aiColor3D color(0.f, 0.f, 0.f);
	this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_SPECULAR, color);
	return color;
}

aiColor3D MaterialHandler::GetTransparentColor(const unsigned int materialIndex) const
{
	aiColor3D color(0.f, 0.f, 0.f);
	this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
	return color;
}

aiBlendMode MaterialHandler::GetBlendMode(const unsigned int materialIndex) const
{
	aiBlendMode blend_mode = aiBlendMode_Default;
	this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_COLOR_TRANSPARENT, blend_mode);
	return blend_mode;
}

float MaterialHandler::GetShininess(const unsigned int materialIndex) const
{
	float shininess = 0.f;
	this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_SHININESS, shininess);
	return shininess;
}

float MaterialHandler::GetShininessStrength(const unsigned int materialIndex) const
{
	float strength = 1.0f;
	this->mpScene->mMaterials[materialIndex]->Get(AI_MATKEY_SHININESS_STRENGTH, strength);
	return strength;
}

float MaterialHandler::GetOpacity(const unsigned int materialIndex) const
{
	float opacity = 1.0f;
	this->mpScene->mMaterials[materialIndex]->Get(
		AI_MATKEY_OPACITY, opacity);
	return opacity;
}

std::string MaterialHandler::GetDiffuseTexturePath(const unsigned int materialIndex) const
{
	aiString texture_path;
	this->mpScene->mMaterials[materialIndex]->Get(
		AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_path);
	return std::string(texture_path.data);
}

aiTextureMapMode MaterialHandler::GetMappingModeU(const unsigned int materialIndex) const
{
	aiTextureMapMode mapping_mode = aiTextureMapMode_Wrap;
	this->mpScene->mMaterials[materialIndex]->Get(
		AI_MATKEY_MAPPINGMODE_U(aiTextureType_DIFFUSE, 0), mapping_mode);
	return mapping_mode;
}

aiTextureMapMode MaterialHandler::GetMappingModeV(const unsigned int materialIndex) const
{
	aiTextureMapMode mapping_mode = aiTextureMapMode_Wrap;
	this->mpScene->mMaterials[materialIndex]->Get(
		AI_MATKEY_MAPPINGMODE_V(aiTextureType_DIFFUSE, 0), mapping_mode);
	return mapping_mode;
}

unsigned int MaterialHandler::GetNumberOfMaterials() const
{
	return this->mNumberOfMaterials;
}

MaterialStruct MaterialHandler::GetMaterialStruct(const unsigned int materialIndex) const
{
	if (materialIndex < this->mMaterialStructVector.size())
	{
		return this->mMaterialStructVector[materialIndex];
	}
	return MaterialStruct();
}

MaterialStruct MaterialHandler::mCreateMaterialStruct(const unsigned int materialIndex)
{
	MaterialStruct to_return;
	aiColor3D diffuseC = this->GetDiffuseColor(materialIndex);
	aiColor3D specularC = this->GetSpecularColor(materialIndex);
	aiColor3D ambientC = this->GetAmbientColor(materialIndex);
	float exp = this->GetShininess(materialIndex);
	float opacity = this->GetOpacity(materialIndex);
	to_return =
	{
		diffuseC.r, diffuseC.g, diffuseC.b,
		ambientC.r, ambientC.g, ambientC.b,
		specularC.r, specularC.g, specularC.b,
		exp, opacity
	};
	return to_return;
}
