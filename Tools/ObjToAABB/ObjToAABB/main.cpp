#include <iostream> 
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
//#include "../../../BIS/BIS/Picking.h"

#define NR_ROOM 18

using namespace std;

struct v3 {
	float x, y, z;
};
struct AABB {
	struct Extrema {
		float min, max;
	};
	Extrema x, y, z;
};

AABB MakeBoundingBox(int currIndex, vector<v3> &vectorList)
{
	v3 &currVert = vectorList[0];
	AABB retAABB;
	retAABB.x.max = retAABB.x.min = currVert.x;
	retAABB.y.max = retAABB.y.min = currVert.y;
	retAABB.z.max = retAABB.z.min = currVert.z;

	for (unsigned int i = 1; i < 8; i++)
	{
		currVert = vectorList[i];

		if (retAABB.x.max < currVert.x)
			retAABB.x.max = currVert.x;
		else if (retAABB.x.min > currVert.x)
			retAABB.x.min = currVert.x;

		if (retAABB.y.max < currVert.y)
			retAABB.y.max = currVert.y;
		else if (retAABB.y.min > currVert.y)
			retAABB.y.min = currVert.y;

		if (retAABB.z.max < currVert.z)
			retAABB.z.max = currVert.z;
		else if (retAABB.z.min > currVert.z)
			retAABB.z.min = currVert.z;
	}
	return retAABB;
}

int main() {
	string line;
	vector<v3> vectorList;
	AABB boundingList[NR_ROOM];
	int currIndex = -1;

	ifstream myfile("All.obj");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			stringstream ss(line);
			string ind;
			ss >> ind;
			if (ind == "o")
			{
				int test;
				ss >> ind;
				ind.erase(0, 4);
				ss = stringstream(ind);
				ss >> test;
				test--;
				if (currIndex != -1) {
					boundingList[currIndex] = MakeBoundingBox(currIndex, vectorList);
					vectorList.clear();
				}
				currIndex = test;
			}
			else if (ind == "v")
			{
				v3 v;
				ss >> v.x >> v.y >> v.z;
				vectorList.push_back(v);
			}
		}
		boundingList[currIndex] = MakeBoundingBox(currIndex, vectorList);
	}
	cout << "Successful";
	myfile.close();
	ofstream outfile("bound.dat", ofstream::binary);
	int tempSize = NR_ROOM;
	outfile.write((char*)&tempSize, sizeof(int));
	outfile.write((char*)boundingList, sizeof(AABB) * NR_ROOM);
	outfile.close();
	ifstream infile("bound.dat", ifstream::binary);
	int sizeList = 0;
	infile.read((char*)&sizeList, sizeof(int));
	AABB* testList = new AABB[sizeList];
	infile.read((char*)testList, sizeof(AABB) * sizeList);
	infile.close();
	cin.ignore();
	return 0;
}