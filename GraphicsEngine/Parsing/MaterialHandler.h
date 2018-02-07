#pragma once
#include "Mesh.h"
class MaterialHandler
{
public:
	MaterialHandler(const aiScene *scene);

	~MaterialHandler();


	std::string GetMaterialName(const unsigned int materialIndex) const;
	// aiColor3D has r g b components, operators and IsBlack() function
	aiColor3D GetDiffuseColor(const unsigned int materialIndex) const;
	aiColor3D GetAmbientColor(const unsigned int materialIndex) const;
	aiColor3D GetSpecularColor(const unsigned int materialIndex) const;
	aiColor3D GetTransparentColor(const unsigned int materialIndex) const;

	aiBlendMode GetBlendMode(const unsigned int materialIndex) const;
	// aiBlendMode_Default or
	// aiBlendMode_Additive
	// http://assimp.sourceforge.net/lib_html/material_8h.html#ae1236da1ccfbf4f23bb490287a4d61ac

	float GetOpacity(const unsigned int materialIndex) const;
	float GetShininess(const unsigned int materialIndex) const; /* The exponent
																used for phong
																specular shading*/
	float GetShininessStrength(const unsigned int materialIndex) const;

	std::string GetDiffuseTexturePath(const unsigned int materialIndex) const;
	//! NOTE: This function assumes there is only one texture per material.


	aiTextureMapMode GetMappingModeU(const unsigned int materialIndex) const;
	aiTextureMapMode GetMappingModeV(const unsigned int materialIndex) const;

	/* Possible values are
	aiTextureMapMode_Wrap --- Classic wrapping
	aiTextureMapMode_Clamp --- Clamp values outside 0-1 to 0 and 1
	aiTextureMapMode_Decal --- Ignore texture if uv is outside 0-1
	aiTextureMapMode_mirror --- "A texture coordinate u|v becomes u%1|v%1
							if (u-(u%1))%2 is zero and 1-(u%1)|1-(v%1) otherwise. */




private:
	const aiScene* mpScene;

};