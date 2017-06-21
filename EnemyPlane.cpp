#include "EnemyPlane.h"
#include <cstdlib>

EnemyPlane::EnemyPlane()
{
}

EnemyPlane::EnemyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, EnemyType type, int life):
    Plane(x, y, imageFile, scene, life, ENEMY)
{
    this->type = type;
}

pair<int,int> EnemyPlane::updatePosition()
{
    return make_pair(x(), y()+2);
}
