#pragma once
#include "Camera.h"
#include "Maze.h"
class Cube
{
private:
	Cube();
	~Cube();
	static Cube *cube;
public:
	 std::vector<GLfloat> getFace(Point3 center, GLfloat length, direction dir);
	 static Cube *getInstance();
	 bool judgeInside(Point3 center, GLfloat length, direction dir, Point3 eyePos);
};

