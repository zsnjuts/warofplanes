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
#include "Plane.h"
#include "MyPlane.h"
#include "EnemyPlane.h"
using namespace std;

/* changePosition函数的返回值：未执行，正常，发生碰撞未耗尽生命值，碰撞且耗尽生命值 */
enum ChangePosFlag { NOCHANGE, CHANGED, CRASH };

class Control
{
	
public:
	Control();
	Control(int boardWidth, int boardHeight, int boardEntension, int enemyNumber,
		int myLife, int myBulletPower, int myBulletSpeed, 
		int enemyLife, int enemyBulletSpeed, int enemyBulletPower);

private:
	int boardWidth;
	int boardHeight;
	int screenWidth;
	int screenHeight;
	int myLife;
	int myBulletPower;
	int myBulletSpeed;
	int enemyLife;
	int enemyBulletSpeed;
	int enemyBulletPower;
	char **screen;
	MyPlane myplane;
	vector<EnemyPlane *> enemyplanes;

	void refreshScreen(); //刷新屏幕
	bool generateEnemyPlane(); //生成一架敌机
	
	int judgeCrash(int aX, int aY, int aHeight, int aWidth, char *aShape,
		int bX, int bY, int bHeight, int bWidth, char *bShape); //判断两飞机是否相撞，返回相撞点的个数
	bool changePlanePosition(Plane *plane, int newX, int newY);
	bool updateEnemyPlanes(); //根据敌机方向更新敌机位置
	
	/* 判断子弹是否击中飞机 */
	bool judgeHit(Bullet * bullet, int bulletNewX, int bulletNewY,
		int planeX, int planeY, int planeHeight, int planeWidth, char *planeShape); 
	bool changeBulletPosition(Bullet *bullet, int newX, int newY); //改变子弹位置，返回子弹是否还在战场
	bool updateBullets(); //更新所有子弹位置
	void shootEnemyBullets(); //所有敌机发射子弹
	void updateMyBullets(); //更新玩家所有子弹位置
	void updateEnemyBullets(); //更新敌机所有子弹位置
	void shootBullet(); //玩家飞机发射子弹
	bool keyboardHandle(char cmd);
	void run();
	
};

#endif // !CONTROL_H
