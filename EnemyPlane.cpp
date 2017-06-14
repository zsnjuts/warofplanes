#include "EnemyPlane.h"
#include <cstdlib>

EnemyPlane::EnemyPlane()
{
}

EnemyPlane::EnemyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life):
    Plane(x, y, imageFile, scene, life, ENEMY)
{
}

pair<int,int> EnemyPlane::updatePosition()
{
/*	int newX = x();
    int newY = y();
    switch (DOWN) //随机生成方向rand() % (RIGHT + 1)
	{
	case EnemyPlane::UP: newX = x - 1; break;
	case EnemyPlane::DOWN: newX = x + 1; break;
	case EnemyPlane::LEFT: newY = y - 1; break;
	case EnemyPlane::RIGHT: newY = y + 1; break;
	default: break;
    }*/

    return make_pair(x(), y()+5);
}
