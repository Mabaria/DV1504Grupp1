#include "Test_AssImp.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

namespace
{
	void PrintStatus(const aiScene *&scene,
		std::string pFile,
		Assimp::Importer importer)
	{
		if (!scene)
		{
			std::cout << "Scene creation failed!" << std::endl;
			std::cout << importer.GetErrorString() << std::endl;
			system("pause");
		}
		else
		{
			// std::boolalpha makes bools print as "true/false", neat huh?
			std::cout << "Scene succesfully created!" << std::endl;
			std::cout << "File: " << pFile << std::endl;
			std::cout << "Has Materials: " << std::boolalpha << scene->HasMaterials() << std::endl;
			std::cout << "Has Meshes: " << std::boolalpha << scene->HasMeshes() << std::endl;
			std::cout << "Has Textures: " << std::boolalpha << scene->HasTextures() << std::endl;
			std::cout << "Has Cameras: " << std::boolalpha << scene->HasCameras() << std::endl;
			std::cout << "Has Lights: " << std::boolalpha << scene->HasLights() << std::endl;
			system("pause");
		}
	}
}

bool TestAssimp() {
	// Usage of this library is quite simple
	// The main object is the const aiScene* object, which contains everything from
	// a loaded file. I recommend peeking at the scene reference to see everything
	// it can contain. 
	//
	// Using the Assimp::Importer class an aiScene object is created and filled with
	// the good stuff. Useful member variables include:
	// mMeshes - Array of meshes from the loaded file
	// mMaterials - Array of materials for the meshes. These are indexed in the mesh objects
	// 
	// The mMeshes array contains aiMesh objects - http://assimp.sourceforge.net/lib_html/structai_mesh.html
	// Useful things here include
	// mVertices - Vertex positions
	// mNormals - Vertex normals
	// mTextureCoords - UV coords of vertices
	// mFaces - Contains the index information for each face
	Assimp::Importer importer;
	std::string pFile = "../../Models/spider.obj";

	//! Scene reference: http://assimp.sourceforge.net/lib_html/structai_scene.html
	const aiScene *scene = importer.ReadFile(pFile,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_OptimizeMeshes |
		aiProcess_ConvertToLeftHanded);

	//! All flags, see http://assimp.sourceforge.net/lib_html/postprocess_8h.html



	::PrintStatus(scene, pFile, importer);

	pFile = "../../Models/4Cubes4Mats_248.blend";
	Assimp::Importer importer2;
	const aiScene *scene2 = importer2.ReadFile(pFile,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_OptimizeMeshes |
		aiProcess_ConvertToLeftHanded);

	::PrintStatus(scene2, pFile, importer2);

	if (scene)
	{
		if (scene->HasMeshes())
		{
			std::cout << "Scene 1 numMeshes: " << scene->mNumMeshes << std::endl;
			std::cout << "Scene 1 Mesh 1 numFaces: " << scene->mMeshes[0]->mNumFaces << std::endl;
			std::cout << "Scene 1 Mesh 1 Face 1 indices: " << std::endl;
			if (scene->mMeshes[0]->HasFaces())
			{
				for (unsigned int i = 0; i < scene->mMeshes[0]->mFaces[0].mNumIndices; i++)
				{
					std::cout << scene->mMeshes[0]->mFaces[0].mIndices[i] << std::endl;
				}
			}
		}
		system("pause");
	}

	if (scene2)
	{
		std::cout << "Scene 2 Mesh 1 Face 1  indices: " << std::endl;
		if (scene2->HasMeshes())
		{
			for (unsigned int i = 0; i < scene2->mMeshes[0]->mFaces[0].mNumIndices; i++)
			{
				std::cout << scene2->mMeshes[0]->mFaces[0].mIndices[i] << std::endl;
			}
		}
		system("pause");
	}


	return true;
}