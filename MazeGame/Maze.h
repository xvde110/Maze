#pragma once
enum direction
{
	S, W, N, E
};
class cell {
public:
	//0����û��ǽ 1������ǽ  N W S E

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
	//�����Թ�
	void createMaze();
	//�ж�ǽ�ǲ��Ǳ߽�
	bool isEdg(wall w);
	//�ж��ܵ���ĵ����Ƿ����p��
	int contain(point p);
	//��ö����ǽ
	wall oppoWall(wall w);
	//��w�����ڵ�ǽ����ǽ������
	void addWall(point p);
	//�ж������Ƿ���ǽ
	bool hasWall(GLint x, GLint y, direction dir);
};

