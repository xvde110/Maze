#pragma once
enum direction
{
	S, W, N, E
};
class cell {
public:
	//0代表没有墙 1代表有墙  N W S E

	/*
			0
	       ---
	    1 |   | 3
		   ---
		    2
	*/

	int door[4]; 
	cell() {
		for (int i = 0; i < 4; i++)
			door[i] = 1;
	}
};
class wall {
public:
	int x, y, dir;
	wall() {}
	wall(int a, int b, int c) { x = a; y = b; dir = c; }
};
class point {
public:
	int x, y;
	point() { x = y = 0; }
	point(int a, int b) { x = a; y = b; }
};
class Maze
{
public:
	Maze(GLint w,GLint h);
	~Maze();
public:
	int height;
	int width;
	cell **map;
	std::vector<point> canReach;
	std::vector<wall> walls;
	int currentX, currentY;
public:
	//创建迷宫
	void createMaze();
	//判断墙是不是边界
	bool isEdg(wall w);
	//判断能到达的点中是否包含p点
	int contain(point p);
	//获得对面的墙
	wall oppoWall(wall w);
	//将w点所在的墙加入墙集合中
	void addWall(point p);
	//判断四周是否有墙
	bool hasWall(GLint x, GLint y, direction dir);
};

