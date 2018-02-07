#include "MaterialHandler.h"

MaterialHandler::MaterialHandler(const aiScene * scene)
{
}

MaterialHandler::~MaterialHandler()
{
}

aiColor3D MaterialHandler::GetDiffuseColor() const
{
	return aiColor3D();
}

aiColor3D MaterialHandler::GetAmbientColor() const
{
	return aiColor3D();
}

aiColor3D MaterialHandler::GetSpecularColor() const
{
	return aiColor3D();
}

float MaterialHandler::GetShininess() const
{
	return 0.0f;
}

float MaterialHandler::GetOpacity()
{
	return 0.0f;
}
