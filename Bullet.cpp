#include "Bullet.h"
#include <iostream>
using namespace std;

Bullet::Bullet()
{
}

Bullet::Bullet(WarPart part, int x, int y, const string &imageFile, const QPointF &dir, int power):
    Object(part, imageFile)
{
    setPos(x, y);
	this->dir = dir;
	this->power = power;
}

pair<qreal, qreal> Bullet::updatePosition()
{
    return make_pair(x()+dir.x(), y()+dir.y());
}

bool Bullet::hit(QGraphicsScene *scene)
{
	if (power <= 0)
		return false;
	else if (--power <= 0) //杀伤力在此次hit之后耗尽
	{
        delScreen(scene);
		return false;
	}
	else
        return true;
}
