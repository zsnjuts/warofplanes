#ifndef CONTROL_H
#define CONTROL_H

#include <ctime>
#include <vector>
#include <cassert>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <windows.h>

#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "Plane.h"
#include "MyPlane.h"
#include "EnemyPlane.h"
using namespace std;

class Control: public QGraphicsScene
{
    Q_OBJECT
public:
	Control();
    Control(int boardWidth, int boardHeight, int enemyNumber,
        const string &myPlaneImageFile, int myLife,
                     const string &myBulletImageFile, int myBulletPower, int myBulletSpeed,
        const string &enemyPlaneImageFile, int enemyLife,
                     const string &enemyBulletImageFile, int enemyBulletSpeed, int enemyBulletPower);

protected:
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    string myPlaneImageFile;
	int myLife;

    string myBulletImageFile;
    int myBulletImageScaleHeight;
	int myBulletPower;
	int myBulletSpeed;

    string enemyPlaneImageFile;
	int enemyLife;

    string enemyBulletImageFile;
    int enemyBulletImageScaleHeight;
	int enemyBulletSpeed;
	int enemyBulletPower;

    int myBulletShootTimerId;
    int enemyBulletShootTimerId;
    int allBulletMoveTimerId;
    int enemyPlaneMoveTimerId;
    int enemyPlaneGenerateTimerId;

    MyPlane *myplane;
    vector<Bullet*> mybullets;
    vector<EnemyPlane *> enemyplanes;
    vector<Bullet*> enemybullets;

    QGraphicsRectItem *lifeFrameBar;
    QGraphicsRectItem *lifeBar;
    QGraphicsRectItem *skillFrameBar;
    QGraphicsRectItem *skillBar;

    QMediaPlayer *player;
    QMediaPlaylist *playList;

	bool generateEnemyPlane(); //生成一架敌机
	
	bool changePlanePosition(Plane *plane, int newX, int newY);
	bool updateEnemyPlanes(); //根据敌机方向更新敌机位置
	
	bool changeBulletPosition(Bullet *bullet, int newX, int newY); //改变子弹位置，返回子弹是否还在战场
	void shootEnemyBullets(); //所有敌机发射子弹
	void updateMyBullets(); //更新玩家所有子弹位置
	void updateEnemyBullets(); //更新敌机所有子弹位置
	void shootBullet(); //玩家飞机发射子弹

    bool isPause; //游戏是否暂停
    QGraphicsWidget *maskWidget; //遮罩面板
    QGraphicsTextItem *gamePausedText; //游戏暂停显示文本
    void pauseGame(); //暂停游戏
	
};

#endif // !CONTROL_H
