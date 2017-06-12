#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H

#include "Plane.h"

class EnemyPlane :protected Plane
{
	friend class Control;
	enum Direction { UP, DOWN, LEFT, RIGHT };
public:
	EnemyPlane();
	EnemyPlane(int x, int y);
	EnemyPlane(int x, int y, int width, int height, const char *shape);
	EnemyPlane(int x, int y, int width, int height, const char *shape, char **screen);
	EnemyPlane(int x, int y, int width, int height, const char *shape, char **screen, int life);
	pair<int,int> updatePosition();

private:
	
};

#endif // !ENEMYPLANE_H

