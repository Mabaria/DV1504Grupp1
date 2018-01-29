#include "Test_Mesh.h"
#include "Mesh.h"
#include <iostream>
bool TestMesh()
{
	std::string filePath = "../../Models/OBJTEST2.obj";
	Mesh* testMesh1;
	try
	{
		testMesh1 = new Mesh(filePath);

	}
	catch (std::string e)
	{
		std::cout << e << std::endl;
		system("pause");
		return false;
	}
	std::cout << "Mesh created successfully!" << std::endl;
	system("pause");
	/* Outdated test stuff
	std::vector<std::vector<unsigned int>> *indices =
		testMesh1->GetIndexVectorsPtr();

	std::cout << "Printing indices for submesh #1: " << std::endl;
	for (unsigned int i = 0; i < indices[0].size(); i++)
	{
		std::cout << indices[0][0][i] << std::endl;
	}
	*/

	std::vector<std::vector<unsigned int>> test = testMesh1->GetIndexVectors();
	std::vector<std::vector<Vertex>> test2 = testMesh1->GetVertexVectors();
	system("pause");
	delete testMesh1;
	return true;
}
