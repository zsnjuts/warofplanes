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

protected:
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    string myPlaneImageFile;
	int myLife;
    int mySkill;

    string myBulletImageFile;
	int myBulletPower;

    string enemyPlaneImageFile;
	int enemyLife;

    string enemyBulletImageFile;
	int enemyBulletPower;

    string bossImageFile;
    int bossLife;

    string bossBulletImageFile;
    int bossBulletPower;

    int myBulletShootTimerId;
    int enemyBulletShootTimerId;
    int allBulletMoveTimerId;

    QPointF myPlaneMove;
    int myPlaneMoveTimerId;
    int enemyPlaneMoveTimerId;
    int enemyPlaneGenerateTimerId;
    int bossGenerateTimeId;

    string lifeSupplyImageFile;

    MyPlane *myplane;
    vector<Bullet*> mybullets;
    vector<EnemyPlane *> enemyplanes;
    vector<Bullet*> enemybullets;
    vector<Object*> lifesupplys;

    QGraphicsRectItem *lifeFrameBar;
    QGraphicsRectItem *lifeBar;
    QGraphicsRectItem *skillFrameBar;
    QGraphicsRectItem *skillBar;

    QMediaPlayer *player;
    QMediaPlaylist *playList;

    bool generateEnemyPlane(); //生成一架敌机
    bool generateBoss(); //生成Boss
	
	bool changePlanePosition(Plane *plane, int newX, int newY);
	bool updateEnemyPlanes(); //根据敌机方向更新敌机位置
	
	bool changeBulletPosition(Bullet *bullet, int newX, int newY); //改变子弹位置，返回子弹是否还在战场
	void shootEnemyBullets(); //所有敌机发射子弹
	void updateMyBullets(); //更新玩家所有子弹位置
	void updateEnemyBullets(); //更新敌机所有子弹位置
	void shootBullet(); //玩家飞机发射子弹

    void updateBar(QGraphicsRectItem *bar, QGraphicsRectItem *frameBar, const QPointF &pos, qreal var, const QBrush &brush); //更新血条或技能条

    bool hasStarted; //是否已经开始游戏
    bool isPause; //游戏是否暂停
    int myBulletType; //是否使用技能
    int skillQTimerId; //技能Q使用时间
    int score; //打掉的飞机数

    QGraphicsTextItem *titleText; //游戏标题
    QGraphicsTextItem *authorText; //作者信息
    QGraphicsTextItem *scoreText; //显示当前打掉的飞机数
    QGraphicsTextItem *gameLostText; //玩家死亡显示文本
    QGraphicsTextItem *gameHelpText; //游戏初始帮助
    void welcomeGame(); //初始界面

    QGraphicsWidget *maskWidget; //遮罩面板
    QGraphicsWidget *startGameButton;
    QGraphicsWidget *helpGameButton;
    QGraphicsWidget *continueGameButton;
    QGraphicsWidget *retryGameButton;
    QGraphicsWidget *quitGameButton;

protected slots:
    void startGame(); //开始游戏
    void showHelpMessage(); //显示帮助信息
    void pauseGame(); //暂停游戏
    void loseGame(); //玩家生命值用尽
    void retryGame();  //重新开始
    void quitGame(); //结束游戏
};

#endif // !CONTROL_H
