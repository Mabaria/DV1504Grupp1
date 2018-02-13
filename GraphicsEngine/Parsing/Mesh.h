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
	Mesh(const std::string &filePath); /* Takes a filepath and creates an object
									   using the Assimp scene structure.
									   Throws an std::string containing the
									   error message on failure. */
	/* One Mesh object gives you access to one(1) vertex vector containing all the
	vertices used by all the "submeshes" in this object. The "submeshes" are divided
	by index vectors, and the double vector<vector<unsigned int>> contains one
	unsigned int vector per submesh. These may or may not have different materials
	and will require one indexed drawcall per submesh. 

	A mesh object can exist with only one submesh, however it should not exist
	with 0 meshes.*/
	~Mesh();

	std::vector<std::vector<Vertex>>& GetVertexVectors(); 	/* Returns a pointer to the
												vector containing all vertexvectors */

	std::vector<std::vector<unsigned int>>& GetIndexVectors(); /*
	Returns a pointer to the vector of unsigned int vectors containing indices.
	Contains one vector per sub-mesh.
	Indices are in triangle primitive form, most likely
	Please don't shoot me if they are triangle strip*/
	aiMaterial* GetMaterialPtr(unsigned int materialIndex); /* Get pointer to
															material for given
															material index*/
	unsigned int GetSubmeshMaterialIndex(unsigned int submeshIndex); /*
	Get material index for a given submesh */

	// The Has..() functions assumes that all the submeshes are in the same format
	bool HasNormals() const; // Checks the first submesh for normals
	bool HasTextureCoords() const; // Check the first submesh for texcoords
	unsigned int GetNumMeshes() const; // Returns number of submeshes
	unsigned int GetNumMaterials() const; // Returns number of materials

	const aiScene *GetScenePointer();



private:
	bool ReadFile(const std::string &filePath);
	void PopulateVectors();

	Assimp::Importer mImporter;
	const aiScene *mpScene;
	std::vector<std::vector<Vertex>> mVertexVectors;
	std::vector<std::vector<unsigned int>> mIndexVectors;
	std::vector<unsigned int> mSubmeshMaterialIndexVector;


};