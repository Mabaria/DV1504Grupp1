#include "Mesh.h"
#include <iostream>

bool TestBoat() {
	std::string path = "../../Models/ShipAll.obj";
	Mesh* shipObj;
	try {
		shipObj = new Mesh(path);
	}
	catch (std::string e) {
		std::cout << e << std::endl;
		system("pause");
		return false;
	}
	std::cout << "Mesh loaded." << std::endl;
	system("pause");

	for (unsigned int i = 19; i < 25; i++)
	{
		/*
		shipObj->GetVertexVectorPtr[i].x << ", "
			<< shipObj->GetVertexVectorPtr[i].y << ", "
			<< shipObj->GetVertexVectorPtr[i].z << std::endl;
		*/
	}

	system("pause");
	delete shipObj;
	return true;
}