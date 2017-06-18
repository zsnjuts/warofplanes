#include "Bullet.h"
#include <iostream>
using namespace std;

Bullet::Bullet()
{
}

Bullet::Bullet(WarPart part, int x, int y, const string &imageFile, const QPointF &dir, int power):
    QGraphicsPixmapItem(QPixmap(QString::fromStdString(imageFile)))
{
    this->part = part;
    setPos(x, y);
	this->dir = dir;
	this->power = power;
}

void Bullet::delScreen(QGraphicsScene *scene)
{
    scene->removeItem(this);
    update();
}

void Bullet::synScreen(QGraphicsScene *scene)
{
    if(!scene->items().contains(this))
    {
        scene->addItem(this);
        update();
    }
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
