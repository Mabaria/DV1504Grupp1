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

	std::vector<std::vector<Vertex>> vertVector = shipObj->GetVertexVectors();
	//Todo This gives errors
	/*for (unsigned int i = 19; i < 25; i++)
	{
		std::cout << vertVector[0][i].x << ", "
			<< vertVector[0][i].y << ", "
			<< vertVector[0][i].z << std::endl;
		
	}*/

	system("pause");
	delete shipObj;
	return true;
}