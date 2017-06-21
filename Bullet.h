#ifndef BULLET_H
#define BULLET_H

#include <QtWidgets>

#include "Object.h"
#include <utility>
#include <fstream>
#include <iostream>
using namespace std;

class Bullet: public Object
{
	friend class Control;
public:
	Bullet();
    Bullet(WarPart part, int x, int y, const string &imageFile, const QPointF &dir, int power);
    pair<qreal,qreal> updatePosition();
    bool hit(QGraphicsScene *scene);

private:
    QPointF dir; //子弹方向，同时也控制了子弹速度
	int power; //子弹杀伤力
};

#endif // !BULLET_H

