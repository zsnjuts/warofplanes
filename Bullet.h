#ifndef BULLET_H
#define BULLET_H

#include <QtWidgets>

#include <utility>
#include <fstream>
#include <iostream>
using namespace std;

enum WarPart { ME, ENEMY };
enum BulletState { READY, RUNNING }; //子弹状态：准备发射or已经发射
enum BulletDirection { UP, UPLEFT, UPRIGHT, DOWN, DOWNLEFT, DOWNRIGHT }; //子弹方向

class Bullet: public QGraphicsPixmapItem
{
	friend class Control;
public:
	Bullet();
    Bullet(WarPart part, int x, int y, const string &imageFile, BulletDirection dir, int power, int speed);
    void synScreen(QGraphicsScene *scene);
    void delScreen(QGraphicsScene *scene);
    pair<qreal, qreal> updatePosition();
    bool hit(QGraphicsScene *scene);

private: //TODO：对于子弹来说，(x,y)应为子弹的
    WarPart part;
	BulletDirection dir;
	int power; //子弹杀伤力
	BulletState state;
	int speed; //子弹速度
	//后期可以添加不同形状，不同杀伤力的子弹
};

#endif // !BULLET_H

