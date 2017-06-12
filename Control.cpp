#include "Control.h"
#include <fstream>

static int myPlaneWidth = 5;
static int myPlaneHeight = 3;
static char myPlaneShape[] = "  |  --*-- -+- ";

static int enemyPlaneWidth = 3;
static int enemyPlaneHeight = 3;
static char enemyPlaneShape[] = " - -+- | ";

Control::Control()
{
}

Control::Control(int boardWidth, int boardHeight, int boardEntension, int enemyNumber, 
	int myLife, int myBulletPower, int myBulletSpeed,
	int enemyLife, int enemyBulletSpeed, int enemyBulletPower):
	myplane(boardHeight - myPlaneHeight, boardWidth / 2, myPlaneWidth, myPlaneHeight, myPlaneShape, myLife)
{
	this->myLife = myLife;
	this->myBulletPower = myBulletPower;
	this->myBulletSpeed = myBulletSpeed;
	this->enemyLife = enemyLife;
	this->enemyBulletPower = enemyBulletPower;
	this->enemyBulletSpeed = enemyBulletSpeed;

	/* 打开日志 */
/*	ofstream fout("log.txt");
	fout << "boardHeight: " << boardHeight << "  boardWidth: " << boardWidth << endl;
	fout << "myplane: (" << myplane.x << "," << myplane.y << ")" << endl;
	fout << "generate " << enemyNumber << " enemyplanes" << endl;
	fout.close();*/

	/* 初始化屏幕 */
	this->boardWidth = boardWidth;
	this->boardHeight = boardHeight;
	this->screenWidth = boardWidth + boardEntension + 3;
	this->screenHeight = boardHeight + 3;
	screen = new char *[screenHeight];
	for (int i = 0; i < screenHeight; i++)
	{
		screen[i] = new char[screenWidth];
		for (int j = 0; j < screenWidth; j++)
			screen[i][j] = ' ';
	}
	
	/* 添加边框，边框有且只有1个字符（已写入了Plane中刷新屏幕的代码） */
	for (int j = 0; j < screenWidth; j++)
		screen[0][j] = '-';
	for (int j = 0; j < screenWidth; j++)
		screen[1+boardHeight][j] = '-';
	for (int i = 0; i < screenHeight; i++)
		screen[i][0] = '|';
	for (int i = 0; i < screenHeight; i++)
		screen[i][boardWidth + 1] = '|';
	for (int i = 0; i < screenHeight; i++)
		screen[i][screenWidth - 1] = '|';
	
	/* 添加棋盘标尺 */
/*	for (int i = 0; i < boardHeight; i++)
		screen[1 + i][1 + boardWidth + 1] = i % 10 + '0';
	for (int i = 0; i < boardWidth; i++)
		screen[1 + boardHeight + 1][1 + i] = i % 10 + '0';
*/
	/* 将玩家飞机同步到屏幕 */
	myplane.synScreen(screen);

	/* 添加敌机 */
	for (int i = 0; i < enemyNumber; i++)
		generateEnemyPlane();

	refreshScreen();
	run();
}

void Control::refreshScreen()
{
	char **myInfo = new char *[boardHeight / 2];
	myInfo[0] = new char[screenWidth - boardWidth - 3];
	strcpy(myInfo[0], "Yours: ");
	myInfo[1] = new char[screenWidth - boardWidth - 3];
	sprintf(myInfo[1], "  Life: %d", myplane.life);
	myInfo[2] = new char[screenWidth - boardWidth - 3];
	sprintf(myInfo[2], "  Bullet Power: %d", myBulletPower);
	myInfo[3] = new char[screenWidth - boardWidth - 3];
	sprintf(myInfo[3], "  Bullet Speed: %d", myBulletSpeed);
	for (int i = 0; i < 4; i++)
		strcpy(screen[1 + i] + boardWidth + 3, myInfo[i]);

	char **enemyInfo = new char *[boardHeight / 2];
	enemyInfo[0] = new char[screenWidth - boardWidth - 3];
	strcpy(enemyInfo[0], "Enemy: ");
	enemyInfo[1] = new char[screenWidth - boardWidth - 3];
	sprintf(enemyInfo[1], "  Left: %d", enemyplanes.size());
	enemyInfo[2] = new char[screenWidth - boardWidth - 3];
	sprintf(enemyInfo[2], "  Bullet Power: %d", enemyBulletPower);
	enemyInfo[3] = new char[screenWidth - boardWidth - 3];
	sprintf(enemyInfo[3], "  Bullet Speed: %d", enemyBulletSpeed);
	for (int i = 0; i < 4; i++)
		strcpy(screen[1 + boardHeight / 2 + i] + boardWidth + 3, enemyInfo[i]);

	/* 将光标位置设置为(0,0) */
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0,0 };
	SetConsoleCursorPosition(hOut, pos);

	/* 隐藏光标 */
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);//获取控制台光标信息  
	CursorInfo.bVisible = false; //隐藏控制台光标  
	SetConsoleCursorInfo(hOut, &CursorInfo);//设置控制台光标状态  

	for (int i = 0; i < screenHeight; i++)
	{
		for (int j = 0; j < screenWidth; j++)
			cout << screen[i][j];
		cout << endl;
	}
}

bool Control::generateEnemyPlane()
{
	/* 随机在第一行生成敌机 */
	srand(time(NULL));//初始化时间种子
	int y = rand() % (boardWidth - enemyPlaneWidth); //敌机最左端位置
	int cnt = 100; //记录循环次数，若当前无法生成新敌机，则直接返回，最多查询100次
	while (cnt > 0 && (screen[1 + 1][1 + y] != ' ' 
		|| screen[1 + 1][1 + y + 1] != ' ' 
		|| screen[1 + 1][1 + y + 2] != ' ') ) //若第2行有一个不为空，则需要重新生成
	{
		y = rand() % (boardWidth - enemyPlaneWidth);
		cnt--;
	}
	
	if (cnt < 0)
		return false;
	else
	{
		/* 新增敌机 */
		EnemyPlane *enemy = new EnemyPlane(0, y, enemyPlaneWidth, enemyPlaneHeight, enemyPlaneShape, screen, enemyLife);
		enemyplanes.push_back(enemy);
		return true;
	}
}

int Control::judgeCrash(int aX, int aY, int aHeight, int aWidth, char * aShape, int bX, int bY, int bHeight, int bWidth, char * bShape)
{
	/* 过滤坐标矩形不重合的情况:aBottom<bTop, aTop>bBottom, aRight<bLeft, aLeft>bRight */
	if (aX + aHeight<bX || aX>bX + bHeight || aY + aWidth < bY || aY > bY + bWidth)
		return 0;
//	ofstream fout("log.txt", ios::app);
	int crashPointCnt = 0;
	for (int i = 0; i < aHeight; i++)
		for (int j = 0; j < aWidth; j++)
			if (aShape[i*aWidth + j] != ' ')
				for (int m = 0; m < bHeight; m++)
					for (int n = 0; n < bWidth; n++)
						if (bShape[m*bWidth + n] != ' ' && aX + i == bX + m && aY + j == bY + n)
						{
//							fout << "<" << aX + i << "," << aY + j << ">";
							crashPointCnt++;
						}
//	fout.close();
	return crashPointCnt;
}

bool Control::changePlanePosition(Plane *plane, int newX, int newY)
{
	/* 检查位置是否有变化，无变化则返回NOCHANGE */
	if (plane->x == newX && plane->y == newY)
		return true;

	/* 检查新位置是否合法，不合法则返回NOCHANGE */
	if (newX<0 || newX>boardHeight - plane->height || newY<0 || newY>boardWidth - plane->width)
	{
		if (plane->type == MYPLANE) //玩家飞机不允许出界
			return true;
		else if (plane->type == ENEMYPLANE) //敌机不允许出上界、左界、右界，但出下界之后将被删除 
		{
			if (newX>boardHeight - plane->height)
			{
				plane->delScreen(screen);
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
		if (judgeCrash(newX, newY, plane->height, plane->width, plane->shape,
			(*it)->x, (*it)->y, (*it)->height, (*it)->width, (*it)->shape) > 0)
		{
			/* 若为玩家飞机，两架飞机均crash，生命值都减1 */
			if (plane->type == MYPLANE)
			{
				plane->crash(screen);
				alive = (*it)->crash(screen);
			}
			if (plane->type == ENEMYPLANE) //若同为敌机，则不允许改变位置，NOCHANGE
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

	/* 然后检查是否与玩家飞机碰撞 */
	if (plane->type == ENEMYPLANE) //若此飞机为敌机
	{
		if(judgeCrash(newX, newY, plane->height, plane->width, plane->shape,
			myplane.x, myplane.y, myplane.height, myplane.width, myplane.shape) >0)
		{
			myplane.crash(screen);
			plane->crash(screen);
		}
	}

	/* 若plane存活，则更改坐标并同步屏幕 */
	if (plane->life > 0)
	{
		plane->delScreen(screen);
		plane->x = newX;
		plane->y = newY;
		plane->synScreen(screen);

		/* 此飞机炮管中的子弹位置随飞机更新 */
		for(vector<Bullet*>::iterator it=plane->bullets.begin();it!=plane->bullets.end();it++)
			if ((*it)->state == READY)
			{
				(*it)->x += (newX - plane->x);
				(*it)->y += (newY - plane->y);
			}
	}

	return plane->life > 0;
}

bool Control::updateEnemyPlanes()
{
	/* 若当前敌机少于3，则自动生成敌机，数目随机但小于8 */
	if (enemyplanes.size() < 3)
	{
		int genNum = rand() % 8;
		for (int i = 0; i < genNum; i++)
			generateEnemyPlane();
	}

	shootEnemyBullets(); //所有敌机发射子弹

	/* 所有敌机移动位置 */
	for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); )
	{
		pair<int, int> loc = (*it)->updatePosition();
		if (changePlanePosition(*it, loc.first, loc.second))
			it++;
		else
			it = enemyplanes.erase(it);
	}

	refreshScreen();

	return myplane.life > 0;
}

bool Control::judgeHit(Bullet * bullet, int bulletNewX, int bulletNewY, 
	int planeX, int planeY, int planeHeight, int planeWidth, char * planeShape)
{
	/* 过滤子弹不在飞机矩形中的情况 */
//	if (bulletX<planeX || bulletX>planeX + planeHeight || bulletY<planeY || bulletY>planeY + planeWidth)
	//	return false;

	/* 遍历飞机各点，若有与子弹重合的点则返回true */
	for (int i = 0; i < planeHeight; i++)
		for (int j = 0; j < planeWidth; j++)
			if (planeShape[i*planeWidth + j] != ' ' &&
				((bullet->y == planeY + j && bullet->dir == DOWN && bullet->x < planeX + i && planeX + i <= bulletNewX)
					|| (bullet->y == planeY + j && bullet->dir == UP && bulletNewX <= planeX + i && planeX + i < bullet->x)))
				return true;

	return false;
}

bool Control::changeBulletPosition(Bullet * bullet, int newX, int newY)
{
//	ofstream fout("log.txt", ios::app);
	/* 检查位置是否有变化，无变化则返回true */
	if (bullet->x == newX && bullet->y == newY)
		return true;

	/* 检查子弹是否击中某一飞机 */
	/* 首先检查玩家飞机 */
	bool flag = judgeHit(bullet, newX, newY, myplane.x, myplane.y, myplane.height, myplane.width, myplane.shape);
/*	fout << "[ <" << bullet->x << "," << bullet->y << "> -> <"
		<< newX << "," << newY << "> & (" 
		<< myplane.x << "," << myplane.y <<  ") ]=" << flag << endl;*/
	if (flag)
	{
		bullet->hit(screen);
		myplane.crash(screen);
	}

	/* 然后检查敌机：即使敌机已经没有生命值，也不从enemyplanes中删去，因为此飞机可能还有子弹在运动 */
	for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); it++)
	{
		flag = judgeHit(bullet, newX, newY, (*it)->x, (*it)->y, (*it)->height, (*it)->width, (*it)->shape);
/*		fout << "[ <" << bullet->x << "," << bullet->y << "> -> <"
			<< newX << "," << newY << "> & ("
			<< (*it)->x << "," << (*it)->y << ") ]=" << flag << endl;*/
		if (flag)
		{
			bullet->hit(screen);
			(*it)->crash(screen);
		}
	}

	/* 检查子弹是否与子弹碰撞，碰撞则同时销毁 */
//	fout << "try: <" << bullet->x << "," << bullet->y << "> -> <" << newX << "," << newY << ">  ";
	for(vector<EnemyPlane*>::iterator iter=enemyplanes.begin();iter!=enemyplanes.end();iter++)
		for (vector<Bullet*>::iterator it = (*iter)->bullets.begin(); it != (*iter)->bullets.end();)
		{
			if (*it == bullet) //跳过自己
				it++;
			else
			{
				bool alive = true;
				if ((*it)->x == newX && (*it)->y == newY)
				{
					bullet->hit(screen);
					alive = (*it)->hit(screen);
				}

				if (alive)
				{
					assert(it != (*iter)->bullets.end());
					it++;
				}
				else
				{
	/*				fout << "collide @(" << newX << "," << newY << ") [" 
						<< "(" << bullet->x << "," << bullet->y << ")and"
						<< "(" << (*it)->x << "," << (*it)->y << ")]" << endl;*/
					delete *it;
					it = (*iter)->bullets.erase(it);
				}
			}
		}

	/* 若子弹还具有杀伤力则更新位置并同步屏幕 */
	if (bullet->power > 0)
	{
		/* 若此时子弹试图打出边界，则销毁子弹 */
		if (newX <= 0 || newX >= boardHeight || newY <= 0 || newY >= boardWidth)
		{
			bullet->delScreen(screen);
			return false;
		}
//		fout << "<" << bullet->x << "," << bullet->y << "> -> <" << newX << "," << newY << ">" << endl;
		if (bullet->state == READY) //若子弹还在炮管中，则更新状态，不删除原先位置
			bullet->state = RUNNING;
		else if(bullet->state==RUNNING) //若子弹已在战场则需要删除原先位置
			bullet->delScreen(screen);
		bullet->x = newX;
		bullet->y = newY;
		bullet->synScreen(screen);
	}
	refreshScreen();

//	fout.close();

	return bullet->power>0;
}

bool Control::updateBullets()
{
	updateMyBullets();
	updateEnemyBullets();
	return myplane.life > 0;
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
				new Bullet((*iter)->x + (*iter)->height - 1, (*iter)->y + 1, DOWN, enemyBulletPower, enemyBulletSpeed));
	updateEnemyBullets();
}

void Control::updateMyBullets()
{
	for (vector<Bullet*>::iterator it = myplane.bullets.begin(); it != myplane.bullets.end(); )
	{
		pair<int, int> loc = (*it)->updatePosition();
		if (changeBulletPosition(*it, loc.first, loc.second))
			it++;
		else
		{
			delete *it;
			it = myplane.bullets.erase(it);
		}
	}
}

void Control::shootBullet()
{
	/* 玩家飞机发出新子弹，新子弹在玩家飞机炮管尖端的位置 */
	myplane.bullets.push_back(new Bullet(myplane.x, myplane.y + 2, UP, myBulletPower, myBulletSpeed));

	/* 更新玩家飞机子弹位置 */
	updateMyBullets();
}

bool Control::keyboardHandle(char cmd)
{
	/* 根据按键改变玩家飞机位置 */
	int newX = myplane.x;
	int newY = myplane.y;
	switch (cmd)
	{
	case 'w': newX = myplane.x - 1; break; //上
	case 's': newX = myplane.x + 1; break; //下
	case 'a': newY = myplane.y - 1;	break; //左
	case 'd': newY = myplane.y + 1; break; //右
	case 'm': shootBullet(); return true; //发射子弹
	case 'p': generateEnemyPlane(); return true; //生成新敌机
	case 'o': myplane.life++; return true; //加生命值
	case 'u': myBulletSpeed = max(myBulletSpeed, 0); return true; //子弹速度+
	case 'i': myBulletSpeed++; return true; //子弹速度-
	case 'h': enemyBulletPower = max(enemyBulletPower, 0); return true; //减小敌机子弹威力
	case 'j': enemyBulletPower++; return true; //增加敌机子弹威力
	case 'k': enemyBulletSpeed = max(enemyBulletSpeed, 0);; return true; //减小敌机子弹速度
	case 'l': enemyBulletSpeed++; return true; //增加敌机子弹速度
	default: return true;
	}

	changePlanePosition(&myplane, newX, newY);
	refreshScreen();
	return myplane.life>0;
}

void Control::run()
{	
	clock_t bulletPositionClock = clock();
	clock_t newBulletClock = clock();
	clock_t planePositionClock = clock();
	while (1)
	{
		if (clock() - bulletPositionClock >= 300) //每500毫秒刷新一次子弹位置
		{
			if (!updateBullets())
				break;
			bulletPositionClock = clock();
		}
		
		if (clock() - newBulletClock >= 500) //每1000毫秒发射一次新子弹
		{
			shootEnemyBullets();
			newBulletClock = clock();
		}

		if (clock() - planePositionClock >= 1000) //每2000毫秒更新一次敌机的位置
		{
			if (!updateEnemyPlanes())
				break;
			planePositionClock = clock();
		}

		if (kbhit() && !keyboardHandle(getch()))
			break;

	}
}
