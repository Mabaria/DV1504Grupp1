#include "Mesh.h"

Mesh::Mesh(const std::string &filePath)
{
	if (this->ReadFile(filePath))
	{
		for (unsigned int i = 0; i < this->mpScene->mNumMeshes; i++)
		{
			this->mIndexVectors.push_back(std::vector<unsigned int>());
			this->mVertexVectors.push_back(std::vector<Vertex>());
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

std::vector<std::vector<Vertex>>& Mesh::GetVertexVectors() 
{
	return this->mVertexVectors;
} 

std::vector<std::vector<unsigned int>>& Mesh::GetIndexVectors() 
{
	return this->mIndexVectors;
} 

unsigned int Mesh::GetNumMeshes() const
{
	return this->mpScene->mNumMeshes;
}

unsigned int Mesh::GetNumMaterials() const
{
	return this->mpScene->mNumMaterials;
}

const aiScene * Mesh::GetScenePointer()
{
	return this->mpScene;
}

aiMaterial* Mesh::GetMaterialPtr(unsigned int materialIndex)
{
	if (materialIndex < this->mpScene->mNumMaterials)
	{
		return this->mpScene->mMaterials[materialIndex];
	}
	else
	{
		return nullptr;
	}
}

unsigned int Mesh::GetSubmeshMaterialIndex(unsigned int submeshIndex)
{
	if (submeshIndex < this->mpScene->mNumMeshes) {
		return this->mSubmeshMaterialIndexVector[submeshIndex];
	}
	else
	{
		return -1;
	}
}

bool Mesh::HasNormals() const
{
	return this->mpScene->mMeshes[0]->HasNormals();
}

bool Mesh::HasTextureCoords() const
{
	return this->mpScene->mMeshes[0]->HasTextureCoords(0); //Check channel 0
}

bool Mesh::ReadFile(const std::string &filePath)
{
	// Flags in this function can be selected depending on needs
	//! CUSTOMIZE THIS FUNCTION
	//! Full list of flags: http://assimp.sourceforge.net/lib_html/postprocess_8h.html

	

	this->mpScene = this->mImporter.ReadFile(filePath,
		aiProcess_Triangulate | // Converts all faces to triangles
		aiProcess_JoinIdenticalVertices | // Removes duplicate vertices in the mesh
		aiProcess_SortByPType | // Removes potential useless lines and points from mesh
		aiProcess_OptimizeMeshes | // Reduces total mesh count by combining
		aiProcess_SplitLargeMeshes | // Splits large meshes
		aiProcess_ConvertToLeftHanded); // Flips UVs, flips winding order and 
										// converts all right handed matrices
										// to left handed
	return this->mpScene; // True if successfull, false if not
}

void Mesh::PopulateVectors()
{
	bool has_normals = true;
	bool has_uvs = true;
	float temp_norm_x, temp_norm_y, temp_norm_z;
	float temp_tex_u, temp_tex_v;
	if (this->mpScene->HasMeshes()) {

		for (unsigned int m = 0; m < this->mpScene->mNumMeshes; m++)
		{
			this->mSubmeshMaterialIndexVector.push_back(
				this->mpScene->mMeshes[m]->mMaterialIndex); /* Add the material
															index to storage
															vector */
			for (unsigned int f = 0; f < this->mpScene->mMeshes[m]->mNumFaces; f++)
			{
				for (unsigned int i = 0;
					i < this->mpScene->mMeshes[m]->mFaces[f].mNumIndices;
					i++)
				{
					this->mIndexVectors[m].push_back(
							this->mpScene->mMeshes[m]->mFaces[f].mIndices[i]);
					// For every mesh, for every face, for every index, add that
					// to the vector with indices for that mesh for later use as
					// index buffer data
				}
			}
			has_normals = this->mpScene->mMeshes[m]->HasNormals();
			has_uvs = this->mpScene->mMeshes[m]->HasTextureCoords(0);
			for (unsigned int v = 0; v < this->mpScene->mMeshes[m]->mNumVertices; v++)
			{
				if (has_normals)
				{
					temp_norm_x = this->mpScene->mMeshes[m]->mNormals[v].x;
					temp_norm_y = this->mpScene->mMeshes[m]->mNormals[v].y;
					temp_norm_z = this->mpScene->mMeshes[m]->mNormals[v].z;
				}
				else
				{
					temp_norm_x = 0.0f;
					temp_norm_y = 0.0f;
					temp_norm_z = 0.0f;
				}
				if (has_uvs)
				{
					temp_tex_u = this->mpScene->mMeshes[m]->mTextureCoords[0][v].x;
					temp_tex_v = this->mpScene->mMeshes[m]->mTextureCoords[0][v].y;
					// Bit confused as to exactly how mTextureCoords is laid out
					// but I think it is separated into channels.
					// I am only saving channel 0, which I assume is the regular
					// texcoords, other channels can be used to save other random
					// stuff, like normalmaps.
				}
				else
				{
					temp_tex_u = -1.0f;
					temp_tex_v = -1.0f;
				}
				Vertex new_vert =
				{
					this->mpScene->mMeshes[m]->mVertices[v].x,
					this->mpScene->mMeshes[m]->mVertices[v].y,
					this->mpScene->mMeshes[m]->mVertices[v].z,

					temp_norm_x,
					temp_norm_y,
					temp_norm_z,

					temp_tex_u,
					temp_tex_v

				}; 	
				this->mVertexVectors[m].push_back(new_vert);
				// For every mesh, for every vertex,
				// create a "Vertex" that combines
				// Vertex positions, vertex normals and vertex UVs
			}
		}
	}
}
