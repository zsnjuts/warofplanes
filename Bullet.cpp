#include "Bullet.h"
#include <iostream>
using namespace std;

Bullet::Bullet()
{
}

Bullet::Bullet(WarPart part, int x, int y, const string &imageFile, BulletDirection dir, int power, int speed):
    QGraphicsPixmapItem(QPixmap(QString::fromStdString(imageFile)))
{
    //刚生成子弹的时候不需要出现在屏幕上，因为子弹在炮管中
    this->part = part;
    setPos(x, y);
	this->dir = dir;
	this->power = power;
	this->state = READY;
	this->speed = speed;
}

void Bullet::delScreen(QGraphicsScene *scene)
{
    //qDebug() << "del bullet begin" << part;
    scene->removeItem(this);
    update();
    //qDebug() << "del bullet end" << part;
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
/*    qreal newX = x();
    qreal newY = y();
    switch (dir)
	{
    case UP: newX -= speed; break;
    case UPLEFT: newX -= speed; newY -= speed; break;
	case UPRIGHT: newX = x - speed; newY = y + speed; break;
	case DOWN: newX = x + speed; break;
	case DOWNLEFT: newX = x + speed; newY = y - speed; break;
	case DOWNRIGHT: newX = x + speed; newY = y + speed; break;
	default: break;
    }
    return make_pair(newX, newY+5);*/
    if(part==ME)
        return make_pair(x(), y()-3);
    else
        return make_pair(x(), y()+1);
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
