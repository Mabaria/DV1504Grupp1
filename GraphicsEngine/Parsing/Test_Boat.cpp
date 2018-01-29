#include "Mesh.h"
#include <iostream>

bool TestBoat() {
	std::string path = "../../Models/SecondTry.blend";
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

	std::vector<Vertex> vertVector = shipObj->GetVertexVector();
	for (unsigned int i = 19; i < 25; i++)
	{
		std::cout << vertVector[i].x << ", "
			<< vertVector[i].y << ", "
			<< vertVector[i].z << std::endl;
		
	}

	system("pause");
	delete shipObj;
	return true;
}