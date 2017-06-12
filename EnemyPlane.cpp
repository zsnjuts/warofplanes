#include "EnemyPlane.h"
#include <cstdlib>

EnemyPlane::EnemyPlane()
{
}

EnemyPlane::EnemyPlane(int x, int y) :Plane(x, y)
{
}

EnemyPlane::EnemyPlane(int x, int y, int width, int height, const char * shape) : 
	Plane(x, y, width, height, shape)
{
}

EnemyPlane::EnemyPlane(int x, int y, int width, int height, const char * shape, char **screen) : 
	Plane(x, y, width, height, shape, screen)
{
}
EnemyPlane::EnemyPlane(int x, int y, int width, int height, const char * shape, char ** screen, int life):
	Plane(x, y, width, height, shape, screen, life, ENEMYPLANE)
{
}

pair<int,int> EnemyPlane::updatePosition()
{
	int newX = x;
	int newY = y;
	switch (rand() % (RIGHT + 1)) //随机生成方向rand() % (RIGHT + 1)
	{
	case EnemyPlane::UP: newX = x - 1; break;
	case EnemyPlane::DOWN: newX = x + 1; break;
	case EnemyPlane::LEFT: newY = y - 1; break;
	case EnemyPlane::RIGHT: newY = y + 1; break;
	default: break;
	}

	return make_pair(newX, newY);
}
