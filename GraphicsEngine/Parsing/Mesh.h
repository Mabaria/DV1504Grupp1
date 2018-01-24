#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

struct Vertex
{
	float x, y, z;
	float normalX, normalY, normalZ;
	float texU, texV;
};

class Mesh
{
public:
	Mesh(const std::string &filePath); /* Throws an std::string containing the
									   error message on failure */
	~Mesh();

	std::vector<Vertex>* GetVertexVectorPtr(); 	/* Returns a pointer to the
												vector containing all vertices */

	std::vector<std::vector<unsigned int>>* GetIndexVectorsPtr(); /*
	Returns a pointer to the vector of unsigned int vectors containing indices.
	Contains one vector per sub-mesh.
																  */
	aiMaterial* GetMaterialPtr(unsigned int materialIndex); /* Get pointer to
															material for given
															material index*/
	unsigned int GetSubmeshMaterialIndex(unsigned int submeshIndex); /*
	Get material index for a given submesh */

	bool HasNormals() const; // Checks the first submesh for normals
	bool HasTextureCoords() const; // Check the first submesh for texcoords
	unsigned int GetNumMeshes() const; // Returns number of submeshes
	unsigned int GetNumMaterials() const; // Returns number of materials



private:
	bool ReadFile(const std::string &filePath);
	void PopulateVectors();

	Assimp::Importer mImporter;
	const aiScene *mScene;
	std::vector<Vertex> mVertexVector;
	std::vector<std::vector<unsigned int>> mIndexVectors;
	std::vector<unsigned int> mSubmeshMaterialIndexVector;


};