#include "stdafx.h"
#include "Maze.h"

Maze::Maze(GLint w, GLint h)
{
	width = w; height = h;
	currentX = currentY = 0;

	//给地图动态分配空间
	map = new cell*[w];
	for (int i = 0; i < w; i++)
		map[i] = new cell[h];

	createMaze();
}

Maze::~Maze()
{
	//释放空间
	for (int i = 0; i < width; i++)
		delete map[i];
	delete map;
}

void Maze::createMaze()
{
	//将起点加入可以到达的墙集合
	canReach.push_back(point(0, 0));

	//将一面墙打通
	walls.push_back(wall(0, 0, 3));

	map[0][0].door[3] = 0;

	int count = 15000;

	for (int i = 0; i < count; i++) {
		//从当前所有墙中随机选一个
		int index = rand() % walls.size();
		wall w = walls[index];

		//取到它对面的墙
		wall opW = oppoWall(w);

		//判断对面墙合不合法 不合法就进行下次循环
		if (opW.x < 0 || opW.x >= height || opW.y < 0 || opW.y >= width)
			continue;

		//判断对面墙是不是终点
		if (opW.x == height - 1 && opW.y == width - 1) {
			//如果是终点 打通墙跳出循环
			map[w.x][w.y].door[w.dir] = 0;
			map[opW.x][opW.y].door[opW.dir] = 0;
			break;
		}

		//判断对面的点是不是已经可以到达了
		//如果已经能够到达 删除这个墙进行下次循环
		if (contain(point(opW.x, opW.y)) != -1) {
			walls.erase(walls.begin() + index);
			continue;
		}

		//将能到达的点加入集合
		canReach.push_back(point(opW.x, opW.y));

		//将墙打通
		map[w.x][w.y].door[w.dir] = 0;
		map[opW.x][opW.y].door[opW.dir] = 0;

		//删除这个墙
		walls.erase(walls.begin() + index);

		//添加对面格子的墙
		addWall(point(opW.x, opW.y));
	}


}

bool Maze::isEdg(wall w)
{
	if (w.x == 0 && w.dir == 1) return true;
	if (w.y == 0 && w.dir == 0) return true;
	if (w.x == height - 1 && w.dir == 2) return true;
	if (w.y == width - 1 && w.dir == 3) return true;
	return false;
}

int Maze::contain(point p)
{
	for (int i = 0; i < canReach.size(); i++)
		if (canReach[i].x == p.x && canReach[i].y == p.y)
			return i;
	return -1;
}

wall Maze::oppoWall(wall w)
{
	wall m;
	switch (w.dir) {
	case 0:
		m.x = w.x;
		m.y = w.y - 1;
		m.dir = 2;
		break;
	case 2:
		m.x = w.x;
		m.y = w.y + 1;
		m.dir = 0;
		break;
	case 1:
		m.x = w.x - 1;
		m.y = w.y;
		m.dir = 3;
		break;
	case 3:
		m.x = w.x + 1;
		m.y = w.y;
		m.dir = 1;
		break;
	}
	return m;
}

void Maze::addWall(point p)
{
	wall w[4];
	for (int i = 0; i < 4; i++)
	{
		w[i].x = p.x;
		w[i].y = p.y;
		w[i].dir = i;
	}
	for (int i = 0; i < 4; i++) {
		if ((!isEdg(w[i])) && map[w[i].x][w[i].y].door[w[i].dir] == 1)
			walls.push_back(w[i]);
	}
}

bool Maze::hasWall(GLint x, GLint y, direction dir)
{
	return map[x][y].door[dir] == 1;
}




