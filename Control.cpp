#include "Control.h"

const QPointF scoreTextPos = QPointF(650, 520);
const QPointF LifeBarPos = QPointF(650,550);
const QPointF SkillBarPos = QPointF(650, 570);

const int myBulletShootTimerItv = 300;
const int enemyBulletShootTimerItv = 1000;
const int allBulletMoveTimerItv = 10;
const int enemyPlaneMoveTimerItv = 50;
const int enemyPlaneGenerateTimerItv = 3000;

Control::Control()
{
}

Control::Control(int boardWidth, int boardHeight,
    const string &myPlaneImageFile, int myLife, int mySkill,
                 const string &myBulletImageFile, int myBulletPower,
    const string &enemyPlaneImageFile, int enemyLife,
                 const string &enemyBulletImageFile, int enemyBulletPower)
{
    this->myPlaneImageFile = myPlaneImageFile;
	this->myLife = myLife;
    this->mySkill = mySkill;

    this->myBulletImageFile = myBulletImageFile;
    this->myBulletImageScaleHeight = myBulletImageScaleHeight;
	this->myBulletPower = myBulletPower;

    this->enemyPlaneImageFile = enemyPlaneImageFile;
	this->enemyLife = enemyLife;

    this->enemyBulletImageFile = enemyBulletImageFile;
	this->enemyBulletPower = enemyBulletPower;

    /* 生命值 */
    lifeFrameBar = new QGraphicsRectItem(LifeBarPos.x(), LifeBarPos.y(), myLife*2,5);
    lifeFrameBar->setPen(QPen(Qt::white));
    addItem(lifeFrameBar);
    lifeBar = new QGraphicsRectItem(LifeBarPos.x(), LifeBarPos.y(), myLife*2, 5);
    lifeBar->setBrush(QBrush(Qt::green));
    addItem(lifeBar);

    /* 技能值 */
    skillFrameBar = new QGraphicsRectItem(SkillBarPos.x(),SkillBarPos.y(), myLife*2,5);
    skillFrameBar->setPen(QPen(Qt::white));
    addItem(skillFrameBar);
    skillBar = new QGraphicsRectItem(SkillBarPos.x(), SkillBarPos.y(), mySkill*2, 5);
    skillBar->setBrush(QBrush(Qt::blue));
    addItem(skillBar);

    /* 初始化场景，播放背景音乐 */
    setSceneRect(0,0,boardWidth,boardHeight);
    playList = new QMediaPlaylist;
    playList->addMedia(QUrl::fromLocalFile("music/starwars.mp3"));
    playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop); //单曲循环
    player = new QMediaPlayer(this);
    player->setPlaylist(playList);
    player->play();

    /* 遮罩面板 */
    QWidget *mask = new QWidget;
    mask->setAutoFillBackground(true);
    mask->setPalette(QPalette(QColor(0, 0, 0, 80)));
    mask->resize(width(),height());
    maskWidget = addWidget(mask);
    maskWidget->setPos(0,0);
    maskWidget->setZValue(1); //设置处在z值为0的图形项上方
    maskWidget->hide();

    /* 游戏暂停提示 */
    isPause = false;
    gamePausedText = new QGraphicsTextItem;
    addItem(gamePausedText);
    gamePausedText->setHtml(tr("<font color=white>GAME PAUSED</font>"));
    gamePausedText->setFont(QFont("Courier", 30, QFont::Bold));
    gamePausedText->setPos(250, 250);
    gamePausedText->setZValue(2);
    gamePausedText->hide();

    /* 得分提示 */
    myBulletType = 0; //子弹类型初始化
    score = 0; //得分清零
    scoreText = new QGraphicsTextItem;
    scoreText->setPos(scoreTextPos);
    scoreText->setHtml(tr("<font color=white>SCORE: %1</font>").arg(score));
    scoreText->setFont(QFont("Courier"));
    addItem(scoreText);

    /* 游戏终止提示 */
    gameLostText = new QGraphicsTextItem;
    addItem(gameLostText);
    gameLostText->setHtml(tr("<font color=white>Game Over</font>"));
    gameLostText->setFont(QFont("Courier", 30, QFont::Bold));
    gameLostText->setPos(250, 250);
    gameLostText->setZValue(2);
    gameLostText->hide();

    /* 游戏开始提示 */
    hasStarted = false;
    gameHelpText = new QGraphicsTextItem;
    addItem(gameHelpText);
    QString helpText = tr("<font color=white size=4>Welcome to zs's WAR OF PLANES Game</font><br><br>");
    helpText += tr("<font color=white size=3>You are supposed to aviod being hit by enemy bullets,<br>");
    helpText += tr("and you will score when your bullet hits an enemy plane.<br>");
    helpText += tr("When you collides with an enemy plane,<br>");
    helpText += tr("your life will decline by 1 while the enemy plane also gets hurt.<br>");
    helpText += tr("<br>Here is some help:<br>");
    helpText += tr("<br>Process Bars:<br>");
    helpText += tr("Green Process Bar: You life value<br>");
    helpText += tr("Blue Process Bar: Your skill value<br>");
    helpText += tr("<br>General Operations:<br>");
    helpText += tr("W: Up;    S: Down;    A: Left;    D: Right<br>");
    helpText += tr("Space: Pause or continue the game<br>");
    helpText += tr("<br>Skill Operations:<br>");
    helpText += tr("Q: Shoot 3 bullets at a time, costing 5 skill points<br>");
    helpText += tr("E: Kill all enemy planes at a time, costing 3 skill points<br>");
    helpText += tr("R: Remove all enemy bullets at a time, costing 7 skill points<br>");
    helpText += tr("<br>Please push Enter to start the game");
    helpText += tr("</font>");
    gameHelpText->setHtml(helpText);
    gameHelpText->setFont(QFont("Courier"));
    gameHelpText->setPos(0,0);
    gameHelpText->setZValue(2);
    gameHelpText->hide();

    /* 进入欢迎界面，之后按Enter开始游戏 */
    welcomeGame();
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
    else if(event->timerId()==enemyPlaneGenerateTimerId)
    {
        for(int i=0;i<2;i++)
            generateEnemyPlane();
    }
    else if(event->timerId()==skillQTimerId)
        myBulletType = 0;
}

void Control::keyPressEvent(QKeyEvent *event)
{
    if(hasStarted==false && event->key()!=Qt::Key_Enter) //若游戏未开始，则除Enter外的按键均无效
        return;

    if(event->key()==Qt::Key_Enter) //只有在没有开始游戏的时候按此键才有效
    {
        hasStarted = true;
        gameHelpText->hide();
        maskWidget->hide();
        startGame();
    }
    else if(event->key()==Qt::Key_W)
    {
        myplane->moveBy(0, -10);
        myplane->update();
    }
    else if(event->key()==Qt::Key_S)
    {
        myplane->moveBy(0, 10);
        myplane->update();
    }
    else if(event->key()==Qt::Key_A)
    {
        myplane->moveBy(-10, 0);
        myplane->update();
    }
    else if(event->key()==Qt::Key_D)
    {
        myplane->moveBy(10, 0);
        myplane->update();
    }
    else if(event->key()==Qt::Key_Q && myplane->skill>=5)
    {
        //按Q的技能可以一次发射3个子弹，但是会消耗5点技能
        myBulletType = 1;
        myplane->skill -= 5;
        updateBar(skillBar, skillFrameBar, SkillBarPos, -10, QBrush(Qt::blue));
        skillQTimerId = startTimer(5000); //5秒使用时间
    }
    else if(event->key()==Qt::Key_E && myplane->skill>=3)
    {
        //按E的技能可以打掉所有飞机，消耗3点技能值
        for(vector<EnemyPlane*>::iterator iter=enemyplanes.begin(); iter!=enemyplanes.end(); iter++)
        {
            score++;
            scoreText->setHtml(tr("<font color=white>SCORE: %1</font>").arg(score));
            removeItem(*iter);
            delete *iter;
        }
        enemyplanes.clear();

        myplane->skill -= 3;
        updateBar(skillBar, skillFrameBar, SkillBarPos, -6, QBrush(Qt::blue));
    }
    else if(event->key()==Qt::Key_R && myplane->skill>=7)
    {
        //按R可以消掉所有敌机子弹，消耗7点技能值
        for(vector<Bullet*>::iterator it = enemybullets.begin(); it!= enemybullets.end(); it++)
        {
            removeItem(*it);
            delete *it;
        }
        enemybullets.clear();

        myplane->skill -= 7;
        updateBar(skillBar, skillFrameBar, SkillBarPos, -14, QBrush(Qt::blue));
    }
    else if(event->key()==Qt::Key_Space)
        pauseGame();
}

bool Control::generateEnemyPlane()
{
	/* 随机在第一行生成敌机 */
    srand(time(NULL));//初始化时间种子，之前这个放在循环内会导致卡死
    int x = rand() % (int)width(); //敌机最左端位置
    QPixmap pixmap(QPixmap(QString::fromStdString(enemyPlaneImageFile)));
    while(!items(x, 0, pixmap.width(), pixmap.height(), Qt::IntersectsItemBoundingRect, Qt::DescendingOrder).empty()) //设置相交模式
        x = rand() % (int)width();

    /* 新增敌机 */
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
                updateBar(lifeBar, lifeFrameBar, LifeBarPos, -2, QBrush(Qt::green));
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
        updateBar(lifeBar, lifeFrameBar, LifeBarPos, -2, QBrush(Qt::green));
    }

	/* 若plane存活，则更改坐标并同步屏幕 */
	if (plane->life > 0)
	{
        plane->moveBy(newX-plane->x(), newY-plane->y());
        plane->update();
	}

	return plane->life > 0;
}

bool Control::updateEnemyPlanes()
{
    /* 若当前敌机少于1，则自动生成敌机，数目随机但小于5 */
    if (enemyplanes.size() < 1)
	{
        int genNum = rand() % 5;
		for (int i = 0; i < genNum; i++)
			generateEnemyPlane();
    }

	/* 所有敌机移动位置 */
	for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); )
	{
        //qDebug() << it-enemyplanes.begin() << "  (" << (*it)->x() << "," << (*it)->y() << ")";
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
        //qDebug() << "myplane: " << myplane->life;
        bullet->hit(this);
        myplane->crash(this);
        updateBar(lifeBar, lifeFrameBar, LifeBarPos, -2, QBrush(Qt::green));
        if(myplane->life<=0)
            loseGame();
	}
    else if(bullet->part==ME)
    {
        /* 然后检查敌机：若敌机已经没有生命值，就从enemyplanes中删去 */
        for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); )
        {
            bool alive = true;
            if (bullet->collidesWithItem(*it))
            {
                bullet->hit(this);
                alive = (*it)->crash(this)==false;
                myplane->skill++;
                updateBar(skillBar, skillFrameBar, SkillBarPos, +2, QBrush(Qt::blue));
                score++;
                scoreText->setHtml(tr("<font color=white>SCORE: %1</font>").arg(score));
            }

            if(alive)
                it++;
            else
            {
                delete *it;
                it = enemyplanes.erase(it);
            }
        }
    }

	/* 若子弹还具有杀伤力则更新位置并同步屏幕 */
	if (bullet->power > 0)
	{
		/* 若此时子弹试图打出边界，则销毁子弹 */
        if (newX <= 0 || newX >= width() || newY <= 0 || newY >= height())
		{
            bullet->delScreen(this);
            return false;
		}
        bullet->synScreen(this);
        bullet->moveBy(newX-bullet->x(), newY-bullet->y());
        bullet->update();
	}

	return bullet->power>0;
}

void Control::updateEnemyBullets()
{
    for(vector<Bullet*>::iterator it = enemybullets.begin(); it!=enemybullets.end(); )
    {
        pair<int ,int> loc = (*it)->updatePosition();
        if(changeBulletPosition(*it, loc.first, loc.second))
            it++;
        else
        {
            delete *it;
            it = enemybullets.erase(it);
        }
    }
}

void Control::shootEnemyBullets()
{
    /* 仍存活的敌机发出新子弹，新子弹在敌机炮管外的位置 */
    for (vector<EnemyPlane*>::iterator iter = enemyplanes.begin(); iter != enemyplanes.end(); iter++)
		if ((*iter)->life > 0)
        {
            Bullet *bullet = new Bullet(ENEMY, (*iter)->x()+(*iter)->pixmap().width()/2, (*iter)->y()+(*iter)->pixmap().height()-15,
                                        enemyBulletImageFile, QPointF(0,1), enemyBulletPower);
            enemybullets.push_back(bullet);
            addItem(bullet);
        }
}

void Control::updateMyBullets()
{
    for (vector<Bullet*>::iterator it = mybullets.begin(); it != mybullets.end(); )
	{
		pair<int, int> loc = (*it)->updatePosition();
		if (changeBulletPosition(*it, loc.first, loc.second))
			it++;
		else
		{
			delete *it;
            it = mybullets.erase(it);
		}
	}
}

//标记：这里还需要添加玩家子弹自动发射的逻辑
void Control::shootBullet()
{
    /* 玩家飞机发出新子弹，新子弹在玩家飞机炮管外的位置 */
    if(myBulletType==0)
    {
        Bullet *bullet = new Bullet(ME, myplane->x()+40, myplane->y()-38,
                                    myBulletImageFile, QPointF(0,-3), myBulletPower);
        mybullets.push_back(bullet);
        addItem(bullet);
    }
    else if(myBulletType==1)
    {
        Bullet *bullet1 = new Bullet(ME, myplane->x()+40, myplane->y()-38,
                                    myBulletImageFile, QPointF(-3,-3), myBulletPower);
        mybullets.push_back(bullet1);
        bullet1->setRotation(-45);
        addItem(bullet1);

        Bullet *bullet2 = new Bullet(ME, myplane->x()+40, myplane->y()-38,
                                    myBulletImageFile, QPointF(0,-3), myBulletPower);
        mybullets.push_back(bullet2);
        addItem(bullet2);

        Bullet *bullet3 = new Bullet(ME, myplane->x()+40, myplane->y()-38,
                                    myBulletImageFile, QPointF(3,-3), myBulletPower);
        mybullets.push_back(bullet3);
        bullet3->setRotation(45);
        addItem(bullet3);
    }
}

void Control::updateBar(QGraphicsRectItem *bar, QGraphicsRectItem *frameBar, const QPointF &pos, qreal var, const QBrush &brush)
{
    qreal wid = bar->rect().width();
    if(var<0)
        wid = max((qreal)0, wid+var);
    else
        wid = min(frameBar->rect().width(), wid+var);
    bar->setRect(pos.x(), pos.y(), wid, bar->rect().height());
    bar->setBrush(brush);
    bar->update();
}

void Control::pauseGame()
{
    if(!isPause)
    {
        isPause = true;
        killTimer(myBulletShootTimerId);
        killTimer(enemyBulletShootTimerId);
        killTimer(allBulletMoveTimerId);
        killTimer(enemyPlaneMoveTimerId);
        killTimer(enemyPlaneGenerateTimerId);
        maskWidget->show();
        gamePausedText->show();
    }
    else
    {
        isPause = false;
        myBulletShootTimerId = startTimer(myBulletShootTimerItv);
        enemyBulletShootTimerId = startTimer(enemyBulletShootTimerItv);
        allBulletMoveTimerId = startTimer(allBulletMoveTimerItv);
        enemyPlaneMoveTimerId = startTimer(enemyPlaneMoveTimerItv);
        enemyPlaneGenerateTimerId = startTimer(enemyPlaneGenerateTimerItv);
        maskWidget->hide();
        gamePausedText->hide();
    }
}

void Control::loseGame()
{
    killTimer(myBulletShootTimerId);
    killTimer(enemyBulletShootTimerId);
    killTimer(allBulletMoveTimerId);
    killTimer(enemyPlaneMoveTimerId);
    killTimer(enemyPlaneGenerateTimerId);
    maskWidget->show();
    gameLostText->setHtml(tr("<font color=white>Game Over<br>Score: %1</font>").arg(score));
    gameLostText->show();
}

void Control::welcomeGame()
{
    hasStarted = false;
    maskWidget->show();
    gameHelpText->show();
}

void Control::startGame()
{
    /* 设置各动作更新时钟 */
    myBulletShootTimerId = startTimer(myBulletShootTimerItv);
    enemyBulletShootTimerId = startTimer(enemyBulletShootTimerItv);
    allBulletMoveTimerId = startTimer(allBulletMoveTimerItv);
    enemyPlaneMoveTimerId = startTimer(enemyPlaneMoveTimerItv);
    enemyPlaneGenerateTimerId = startTimer(enemyPlaneGenerateTimerItv);

    /* 添加玩家飞机 */
    myplane = new MyPlane(width() / 2, height() / 2, myPlaneImageFile, this, myLife, mySkill);
    myplane->synScreen(this);

    /* 添加敌机 */
    for (int i = 0; i < 3; i++)
        generateEnemyPlane();
}
