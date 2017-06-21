#include "Plane.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

Plane::Plane()
{
}

Plane::Plane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, enum WarPart part):
    Object(part, imageFile)
{
    setPos(x, y);
    this->life = life;
    scene->addItem(this);
    update();
}

bool Plane::crash(QGraphicsScene *scene)
{
	if (life <= 0) //若已经耗尽生命值
		return false;
	else if (--life <= 0) //若生命值此次crash后耗尽
	{
        delScreen(scene);
		return false;
	}
	else
		return true;
}
