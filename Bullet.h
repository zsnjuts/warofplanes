#ifndef BULLET_H
#define BULLET_H

#include <QtWidgets>

#include <utility>
#include <fstream>
#include <iostream>
using namespace std;

enum WarPart { ME, ENEMY };

class Bullet: public QGraphicsPixmapItem
{
	friend class Control;
public:
	Bullet();
    Bullet(WarPart part, int x, int y, const string &imageFile, const QPointF &dir, int power);
    void synScreen(QGraphicsScene *scene);
    void delScreen(QGraphicsScene *scene);
    pair<qreal,qreal> updatePosition();
    bool hit(QGraphicsScene *scene);

private:
    WarPart part;
    QPointF dir; //子弹方向，同时也控制了子弹速度
	int power; //子弹杀伤力
};

#endif // !BULLET_H

