#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H

#include "Plane.h"

enum EnemyType {ORD, BOSS};

class EnemyPlane :protected Plane
{
	friend class Control;
public:
	EnemyPlane();
    EnemyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, EnemyType type, int life);
	pair<int,int> updatePosition();

protected:
    EnemyType type;
	
};

#endif // !ENEMYPLANE_H

