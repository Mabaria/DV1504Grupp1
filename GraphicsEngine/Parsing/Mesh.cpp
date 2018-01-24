#include "Mesh.h"

Mesh::Mesh(const std::string &filePath)
{
	if (this->ReadFile(filePath))
	{
		for (unsigned int i = 0; i < this->mScene->mNumMeshes; i++)
		{
			this->mIndexVectors.push_back(std::vector<unsigned int>());
			// Add vectors for every submesh
		}
		this->PopulateVectors();
	}
	else
	{
		// If the file could not be opened, throw error message.
		throw std::string("Failed to create mesh!\n") + 
			this->mImporter.GetErrorString();
	}
}

Mesh::~Mesh()
{

}

std::vector<Vertex>* Mesh::GetVertexVectorPtr() 
{
	return &this->mVertexVector;
} 

std::vector<std::vector<unsigned int>>* Mesh::GetIndexVectorsPtr() 
{
	return &this->mIndexVectors;
} 

unsigned int Mesh::GetNumMeshes() const
{
	return this->mScene->mNumMeshes;
}

unsigned int Mesh::GetNumMaterials() const
{
	return this->mScene->mNumMaterials;
}

aiMaterial* Mesh::GetMaterialPtr(unsigned int materialIndex)
{
	if (materialIndex < this->mScene->mNumMaterials)
	{
		return this->mScene->mMaterials[materialIndex];
	}
	else
	{
		return nullptr;
	}
}

unsigned int Mesh::GetSubmeshMaterialIndex(unsigned int submeshIndex)
{
	if (submeshIndex > this->mScene->mNumMeshes) {
		return this->mSubmeshMaterialIndexVector[submeshIndex];
	}
	else
	{
		return -1;
	}
}

bool Mesh::HasNormals() const
{
	return this->mScene->mMeshes[0]->HasNormals();
}

bool Mesh::HasTextureCoords() const
{
	return this->mScene->mMeshes[0]->HasTextureCoords(0); //Check channel 0
}

bool Mesh::ReadFile(const std::string &filePath)
{
	// Flags in this function can be selected depending on needs
	//! CUSTOMIZE THIS FUNCTION
	//! Full list of flags: http://assimp.sourceforge.net/lib_html/postprocess_8h.html

	

	this->mScene = this->mImporter.ReadFile(filePath,
		aiProcess_Triangulate | // Converts all faces to triangles
		aiProcess_JoinIdenticalVertices | // Removes duplicate vertices in the mesh
		aiProcess_SortByPType | // Removes potential useless lines and points from mesh
		aiProcess_OptimizeMeshes | // Reduces total mesh count by combining
		aiProcess_ConvertToLeftHanded); // Flips UVs, flips winding order and 
										// converts all right handed matrices
										// to left handed
	return this->mScene; // True if successfull, false if not
}

void Mesh::PopulateVectors()
{
	if (this->mScene->HasMeshes()) {

		for (unsigned int m = 0; m < this->mScene->mNumMeshes; m++)
		{
			for (unsigned int f = 0; f < this->mScene->mMeshes[m]->mNumFaces; f++)
			{
				for (unsigned int i = 0;
					i < this->mScene->mMeshes[m]->mFaces[f].mNumIndices;
					i++)
				{
					this->mIndexVectors[m].push_back(
							this->mScene->mMeshes[m]->mFaces[f].mIndices[i]);
					// For every mesh, for every face, for every index, add that
					// to the vector with indices for that mesh for later use as
					// index buffer data
				}
			}
			for (unsigned int v = 0; v < this->mScene->mMeshes[m]->mNumVertices; v++)
			{
				Vertex new_vert =
				{
					this->mScene->mMeshes[m]->mVertices[v].x,
					this->mScene->mMeshes[m]->mVertices[v].y,
					this->mScene->mMeshes[m]->mVertices[v].z,

					this->mScene->mMeshes[m]->mNormals[v].x,
					this->mScene->mMeshes[m]->mNormals[v].y,
					this->mScene->mMeshes[m]->mNormals[v].z,

					this->mScene->mMeshes[m]->mTextureCoords[0][v].x,
					this->mScene->mMeshes[m]->mTextureCoords[0][v].y

				}; 	// Bit confused as to exactly how mTextureCoords is laid out
					// but I think it is separated into channels.
					// I am only saving channel 0, which I assume is the regular
					// texcoords, other channels can be used to save other random
					// stuff, like normalmaps.
				this->mVertexVector.push_back(new_vert);
				// For every mesh, for every vertex,
				// create a "Vertex" that combines
				// Vertex positions, vertex normals and vertex UVs
			}
		}
	}
}
