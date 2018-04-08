#include "stdafx.h"
#include "Maze.h"

Maze::Maze(GLint w, GLint h)
{
	width = w; height = h;
	currentX = currentY = 0;

	//����ͼ��̬����ռ�
	map = new cell*[w];
	for (int i = 0; i < w; i++)
		map[i] = new cell[h];

	createMaze();
}

Maze::~Maze()
{
	//�ͷſռ�
	for (int i = 0; i < width; i++)
		delete map[i];
	delete map;
}

void Maze::createMaze()
{
	//����������Ե����ǽ����
	canReach.push_back(point(0, 0));

	//��һ��ǽ��ͨ
	walls.push_back(wall(0, 0, 3));

	map[0][0].door[3] = 0;

	int count = 15000;

	for (int i = 0; i < count; i++) {
		//�ӵ�ǰ����ǽ�����ѡһ��
		int index = rand() % walls.size();
		wall w = walls[index];

		//ȡ���������ǽ
		wall opW = oppoWall(w);

		//�ж϶���ǽ�ϲ��Ϸ� ���Ϸ��ͽ����´�ѭ��
		if (opW.x < 0 || opW.x >= height || opW.y < 0 || opW.y >= width)
			continue;

		//�ж϶���ǽ�ǲ����յ�
		if (opW.x == height - 1 && opW.y == width - 1) {
			//������յ� ��ͨǽ����ѭ��
			map[w.x][w.y].door[w.dir] = 0;
			map[opW.x][opW.y].door[opW.dir] = 0;
			break;
		}

		//�ж϶���ĵ��ǲ����Ѿ����Ե�����
		//����Ѿ��ܹ����� ɾ�����ǽ�����´�ѭ��
		if (contain(point(opW.x, opW.y)) != -1) {
			walls.erase(walls.begin() + index);
			continue;
		}

		//���ܵ���ĵ���뼯��
		canReach.push_back(point(opW.x, opW.y));

		//��ǽ��ͨ
		map[w.x][w.y].door[w.dir] = 0;
		map[opW.x][opW.y].door[opW.dir] = 0;

		//ɾ�����ǽ
		walls.erase(walls.begin() + index);

		//��Ӷ�����ӵ�ǽ
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




