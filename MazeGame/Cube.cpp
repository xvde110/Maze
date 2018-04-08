#include "stdafx.h"
#include "Cube.h"
#include <math.h>
Cube::Cube()
{
}


Cube::~Cube()
{
}

std::vector<GLfloat> Cube::getFace(Point3 center, GLfloat length, direction dir)
{

	GLfloat k = 0.25;
	int pos[8][3] = { { -1,-1,1 },{ 1,-1,1 },{ 1,1,1 },{ -1,1,1 },
	{ -1,-1,-1 },{ 1,-1,-1 },{ 1,1,-1 },{ -1,1,-1 } };
	Point3 positions_[8];
	for (int i = 0; i < 8; i++) {
		if (dir == E) {
			positions_[i].set(center.x + (pos[i][0] * length)*k + length,
				center.y + pos[i][1] * length,
				center.z + pos[i][2] * length);
		}
		if (dir == W) {
			positions_[i].set(center.x + (pos[i][0] * length)*k - length,
				center.y + pos[i][1] * length,
				center.z + pos[i][2] * length);
		}
		if (dir == S) {
			positions_[i].set(center.x + pos[i][0] * length,
				center.y + pos[i][1] * length,
				center.z + (pos[i][2] * length)*k - length);
		}
		if (dir == N) {
			positions_[i].set(center.x + pos[i][0] * length,
				center.y + pos[i][1] * length,
				center.z + (pos[i][2] * length)*k + length);
		}
	}
	std::vector<GLfloat> v(24 * 8, 0);
	GLint arrayIndex[] = { 0,1,2,3,7,6,5,4,5,1,0,4,6,2,1,5,7,3,2,6,4,0,3,7 };
	for (int i = 0; i < 24; i++) {
		//position
		v[i * 8 + 0] = positions_[arrayIndex[i]].x;
		v[i * 8 + 1] = positions_[arrayIndex[i]].y;
		v[i * 8 + 2] = positions_[arrayIndex[i]].z;
		//texcoord
		switch (i % 4)
		{
		case 0:v[i * 8 + 3] = 1.0; v[i * 8 + 4] = 0.0; break;
		case 1:v[i * 8 + 3] = 0.0; v[i * 8 + 4] = 0.0; break;
		case 2:v[i * 8 + 3] = 0.0; v[i * 8 + 4] = 1.0; break;
		case 3:v[i * 8 + 3] = 1.0; v[i * 8 + 4] = 1.0; break;
		}
		switch (i/4)
		{
		case 0:v[i * 8 + 5] = 0.0; v[i * 8 + 6] = 0.0; v[i * 8 + 7] = 1.0; break;
		case 1:v[i * 8 + 5] = 0.0; v[i * 8 + 6] = 0.0; v[i * 8 + 7] = -1.0; break;
		case 2:v[i * 8 + 5] = 0.0; v[i * 8 + 6] = -1.0; v[i * 8 + 7] = 0.0; break;
		case 3:v[i * 8 + 5] = 1.0; v[i * 8 + 6] = 0.0; v[i * 8 + 7] = 0.0; break;
		case 4:v[i * 8 + 5] = 0.0; v[i * 8 + 6] = 1.0; v[i * 8 + 7] = 0.0; break;
		case 5:v[i * 8 + 5] = -1.0; v[i * 8 + 6] = 0.0; v[i * 8 + 7] = 0.0; break;
		}
	}
	return v;
}
Cube* Cube::cube = NULL;
Cube * Cube::getInstance()
{
	if (cube == NULL)
		cube = new Cube();
	return cube;
}
//Åö×²¼ì²â
bool Cube::judgeInside(Point3 center, GLfloat length, direction dir, Point3 eyePos)
{
	GLfloat k = 0.25;
	int pos[8][3] = { { -1,-1,1 },{ 1,-1,1 },{ 1,1,1 },{ -1,1,1 },
	{ -1,-1,-1 },{ 1,-1,-1 },{ 1,1,-1 },{ -1,1,-1 } };
	Point3 positions_[8];
	for (int i = 0; i < 8; i++) {
		if (dir == E) {
			positions_[i].set(center.x + (pos[i][0] * length)*k + length,
				center.y + pos[i][1] * length,
				center.z + pos[i][2] * length);
		}
		if (dir == W) {
			positions_[i].set(center.x + (pos[i][0] * length)*k - length,
				center.y + pos[i][1] * length,
				center.z + pos[i][2] * length);
		}
		if (dir == S) {
			positions_[i].set(center.x + pos[i][0] * length,
				center.y + pos[i][1] * length,
				center.z + (pos[i][2] * length)*k - length);
		}
		if (dir == N) {
			positions_[i].set(center.x + pos[i][0] * length,
				center.y + pos[i][1] * length,
				center.z + (pos[i][2] * length)*k + length);
		}
	}
	GLfloat maxX = 0, maxY = 0, maxZ = 0;
	GLfloat minX = 1000000, minY = 1000000, minZ = 1000000;
	for (int i = 0; i < 8; i++) {
		maxX = max(maxX, positions_[i].x);
		maxY = max(maxY, positions_[i].y);
		maxZ = max(maxZ, positions_[i].z);
		minX = min(minX, positions_[i].x);
		minY = min(minY, positions_[i].y);
		minZ = min(minZ, positions_[i].z);
	}
	GLint off = 2;
	maxX += off; maxY += off; maxZ += off;
	minX -= off; minY -= off; minZ -= off;
	/*std::cout << minX << " " << minY << " " << minZ << std::endl;
	std::cout << maxX << " " << maxY << " " << maxZ << std::endl;
	std::cout << eyePos.x << " " << eyePos.y << " " << eyePos.z << std::endl << std::endl;*/
	if (eyePos.x<maxX && eyePos.y<maxY && eyePos.z<maxZ &&
		eyePos.x>minX && eyePos.y>minY && eyePos.z>minZ)
	{
		return true;
	}
	return false;
}

