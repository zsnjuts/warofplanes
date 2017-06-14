#include "Control.h"

Control::Control()
{
}

Control::Control(int boardWidth, int boardHeight, int enemyNumber,
    const string &myPlaneImageFile, int myLife,
                 const string &myBulletImageFile, int myBulletPower, int myBulletSpeed,
    const string &enemyPlaneImageFile, int enemyLife,
                 const string &enemyBulletImageFile, int enemyBulletSpeed, int enemyBulletPower)
{
    this->myPlaneImageFile = myPlaneImageFile;
	this->myLife = myLife;

    this->myBulletImageFile = myBulletImageFile;
    this->myBulletImageScaleHeight = myBulletImageScaleHeight;
	this->myBulletPower = myBulletPower;
	this->myBulletSpeed = myBulletSpeed;

    this->enemyPlaneImageFile = enemyPlaneImageFile;
	this->enemyLife = enemyLife;

    this->enemyBulletImageFile = enemyBulletImageFile;
    this->enemyBulletImageScaleHeight = enemyBulletImageScaleHeight;
	this->enemyBulletPower = enemyBulletPower;
	this->enemyBulletSpeed = enemyBulletSpeed;

    /* 设置各动作更新时钟 */
    myBulletShootTimerId = startTimer(500); //300
    enemyBulletShootTimerId = startTimer(1000);
    allBulletMoveTimerId = startTimer(10);
    enemyPlaneMoveTimerId = startTimer(1000);

    /* 初始化场景 */
    setSceneRect(0,0,boardWidth,boardHeight);
	
    /* 添加玩家飞机 */
    myplane = new MyPlane(boardWidth / 2, boardHeight / 2, myPlaneImageFile, this, myLife);
    myplane->synScreen(this);

	/* 添加敌机 */
	for (int i = 0; i < enemyNumber; i++)
		generateEnemyPlane();
}

void Control::timerEvent(QTimerEvent *event)
{
    //TODO: 若玩家飞机坠毁，不知道如何结束游戏
    if(event->timerId()==enemyBulletShootTimerId)
        shootEnemyBullets();
    else if(event->timerId()==myBulletShootTimerId)
        shootBullet();
    else if(event->timerId()==allBulletMoveTimerId)
    {
        updateMyBullets();
        updateEnemyBullets();
    }
    else if(event->timerId()==enemyPlaneMoveTimerId)
        updateEnemyPlanes();
}

void Control::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_W)
        myplane->moveBy(0, -10);
    else if(event->key()==Qt::Key_S)
        myplane->moveBy(0, 10);
    else if(event->key()==Qt::Key_A)
        myplane->moveBy(-10, 0);
    else if(event->key()==Qt::Key_D)
        myplane->moveBy(10, 0);
    myplane->update();
}

bool Control::generateEnemyPlane()
{
	/* 随机在第一行生成敌机 */
    int x = rand() % (int)width(); //敌机最左端位置
    while(!items(QPointF(x,0), Qt::IntersectsItemBoundingRect).empty()) //设置相交模式
    {
        srand(time(NULL));//初始化时间种子
        x = rand() % (int)width();
    }

    /* 新增敌机 */
    qDebug() << "gen: " << x;
    EnemyPlane *enemy = new EnemyPlane(x, 0, enemyPlaneImageFile, this, enemyLife);
    enemyplanes.push_back(enemy);
    return true;
}

bool Control::changePlanePosition(Plane *plane, int newX, int newY)
{
    /* 检查位置是否有变化，无变化则直接返回 */
    if (plane->x() == newX && plane->y() == newY)
		return true;

    /* 检查新位置是否合法，不合法则直接返回 */
    if (newX<0 || newX>width() || newY<0 || newY>height())
	{
        if (plane->part == ME) //玩家飞机不允许出界
			return true;
        else if (plane->part == ENEMY) //敌机不允许出上界、左界、右界，但出下界之后将被删除
		{
            if (newY>height())
			{
                plane->delScreen(this);
				return false;
			}
			else
				return true;
		}
	}

	/* 检查新位置是否与某一飞机位置冲突 */
	/* 首先检查是否与敌机碰撞 */
	for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); ) //遍历敌机
	{
		if (plane == *it) //跳过自己
		{
			it++;
			continue;
		}

		bool alive = true; //it所指向的飞机是否还有生命值
        if (plane->collidesWithItem(*it))
		{
			/* 若为玩家飞机，两架飞机均crash，生命值都减1 */
            if (plane->part == ME)
			{
                plane->crash(this);
                alive = (*it)->crash(this);
			}
            if (plane->part == ENEMY) //若同为敌机，则不允许改变位置，NOCHANGE
				return true;
		}

		if (alive)
			it++;
		else //若飞机坠毁，则将此飞机去掉
		{
			delete *it;
			it = enemyplanes.erase(it);
		}
	}

    /* 若此飞机为敌机，且与玩家飞机发生了碰撞 */
    if (plane->part == ENEMY && plane->collidesWithItem(myplane))
    {
        myplane->crash(this);
        plane->crash(this);
    }

	/* 若plane存活，则更改坐标并同步屏幕 */
	if (plane->life > 0)
	{
        plane->moveBy(newX-plane->x(), newY-plane->y());
        plane->update();

		/* 此飞机炮管中的子弹位置随飞机更新 */
		for(vector<Bullet*>::iterator it=plane->bullets.begin();it!=plane->bullets.end();it++)
			if ((*it)->state == READY)
                (*it)->moveBy(newX-plane->x(), newY-plane->y());
	}

	return plane->life > 0;
}

bool Control::updateEnemyPlanes()
{
	/* 若当前敌机少于3，则自动生成敌机，数目随机但小于8 */
/*	if (enemyplanes.size() < 3)
	{
		int genNum = rand() % 8;
		for (int i = 0; i < genNum; i++)
			generateEnemyPlane();
    }*/

	shootEnemyBullets(); //所有敌机发射子弹

	/* 所有敌机移动位置 */
	for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); )
	{
        qDebug() << it-enemyplanes.begin() << "  (" << (*it)->x() << "," << (*it)->y() << ")";
		pair<int, int> loc = (*it)->updatePosition();
		if (changePlanePosition(*it, loc.first, loc.second))
			it++;
		else
			it = enemyplanes.erase(it);
	}

    return myplane->life > 0;
}

bool Control::changeBulletPosition(Bullet * bullet, int newX, int newY)
{
	/* 检查位置是否有变化，无变化则返回true */
    if (bullet->x() == newX && bullet->y() == newY)
		return true;

	/* 检查子弹是否击中某一飞机 */
	/* 首先检查玩家飞机 */
    if (bullet->part==ENEMY && bullet->collidesWithItem(myplane))
	{
        qDebug() << "bullet hit player begin " << bullet->part;
        bullet->hit(this);
        myplane->crash(this);
        qDebug() << "bullet hit player end " << bullet->part;
	}
    else if(bullet->part==ME)
    {
        /* 然后检查敌机：即使敌机已经没有生命值，也不从enemyplanes中删去，因为此飞机可能还有子弹在运动 */
        for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); it++)
            if (bullet->collidesWithItem(*it))
            {
                qDebug() << "collid del bullet begin" << bullet->part;
                bullet->hit(this);
                (*it)->crash(this);
                qDebug() << "collid del bullet end" << bullet->part;
            }
    }

	/* 若子弹还具有杀伤力则更新位置并同步屏幕 */
	if (bullet->power > 0)
	{
		/* 若此时子弹试图打出边界，则销毁子弹 */
        if (newX <= 0 || newX >= width() || newY <= 0 || newY >= height())
		{
            qDebug() << "illegal del bullet begin" << bullet->part;
            bullet->delScreen(this);
            qDebug() << "illegal del bullet end" << bullet->part;
            return false;
		}
		if (bullet->state == READY) //若子弹还在炮管中，则更新状态，不删除原先位置
			bullet->state = RUNNING;
        bullet->synScreen(this);
        bullet->moveBy(newX-bullet->x(), newY-bullet->y());
        bullet->update();
	}

	return bullet->power>0;
}

void Control::updateEnemyBullets()
{
	for (vector<EnemyPlane*>::iterator iter = enemyplanes.begin(); iter != enemyplanes.end(); )
	{
		for (vector<Bullet*>::iterator it = (*iter)->bullets.begin(); it != (*iter)->bullets.end(); )
		{
			pair<int, int> loc = (*it)->updatePosition();
			if (changeBulletPosition(*it, loc.first, loc.second)) //若子弹已毁，则销毁并移除
				it++;
			else
			{
				delete *it;
				it = (*iter)->bullets.erase(it);
			}
		}

		if ((*iter)->life <= 0 && (*iter)->bullets.empty()) //若敌机已毁且没有子弹，则删去此敌机
		{
			delete *iter;
			iter = enemyplanes.erase(iter);
		}
		else
			iter++;
	}
}

void Control::shootEnemyBullets()
{
	/* 仍存活的敌机发出新子弹，新子弹在敌机炮管尖端的位置 */
	for (vector<EnemyPlane*>::iterator iter = enemyplanes.begin(); iter != enemyplanes.end(); iter++)
		if ((*iter)->life > 0)
			(*iter)->bullets.push_back(
                new Bullet(ENEMY, (*iter)->x()+(*iter)->pixmap().width()/2, (*iter)->y()+(*iter)->pixmap().height()-1,
                           enemyBulletImageFile, DOWN, enemyBulletPower, enemyBulletSpeed));
	updateEnemyBullets();
}

void Control::updateMyBullets()
{
    for (vector<Bullet*>::iterator it = myplane->bullets.begin(); it != myplane->bullets.end(); )
	{
		pair<int, int> loc = (*it)->updatePosition();
		if (changeBulletPosition(*it, loc.first, loc.second))
			it++;
		else
		{
			delete *it;
            it = myplane->bullets.erase(it);
		}
	}
}

//标记：这里还需要添加玩家子弹自动发射的逻辑
void Control::shootBullet()
{
	/* 玩家飞机发出新子弹，新子弹在玩家飞机炮管尖端的位置 */
    myplane->bullets.push_back(new Bullet(ME, myplane->x()+40, myplane->y() + 2,
                                          myBulletImageFile, UP, myBulletPower, myBulletSpeed));

	/* 更新玩家飞机子弹位置 */
	updateMyBullets();
}
