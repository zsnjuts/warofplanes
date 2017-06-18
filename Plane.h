#ifndef PLANE_H
#define PLANE_H

#include "Bullet.h"
#include <QtWidgets>
#include <cstring>
#include <vector>
using namespace std;

class Plane: public QGraphicsPixmapItem
{
	friend class Control;
public:
	Plane();
    Plane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, enum WarPart part);
    void delScreen(QGraphicsScene *scene); //在屏幕上将此飞机抹去
    void synScreen(QGraphicsScene *scene); //在屏幕相应位置添加此飞机
    bool crash(QGraphicsScene *scene); //飞机发生碰撞
	
protected:
	int life; //生命值
    enum WarPart part;
};

#endif // !PLANE_H
