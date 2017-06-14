#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H

#include "Plane.h"

class EnemyPlane :protected Plane
{
	friend class Control;
	enum Direction { UP, DOWN, LEFT, RIGHT };
public:
	EnemyPlane();
    EnemyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life);
	pair<int,int> updatePosition();

private:
	
};

#endif // !ENEMYPLANE_H

