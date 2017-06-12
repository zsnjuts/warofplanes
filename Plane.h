#ifndef PLANE_H
#define PLANE_H

#include "Bullet.h"
#include <cstring>
#include <vector>
using namespace std;

enum Type { MYPLANE, ENEMYPLANE };

class Plane
{
	friend class Control;
public:
	Plane();
	Plane(int x, int y);
	Plane(int x, int y, int width, int height, const char *shape);
	Plane(int x, int y, int width, int height, const char *shape, char **screen);
	Plane(int x, int y, int width, int height, const char *shape, int life);
	Plane(int x, int y, int width, int height, const char *shape, int life, Type type);
	Plane(int x, int y, int width, int height, const char * shape, char ** screen, int life);
	Plane(int x, int y, int width, int height, const char *shape, char **screen, int life, Type type);
	void delScreen(char **screen); //在屏幕上将此飞机抹去
	void synScreen(char **screen); //在屏幕相应位置添加此飞机
	bool crash(char **screen); //飞机发生碰撞
	~Plane();
	
protected:
	int x; //最上面行
	int y; //最左侧列
	int width; //宽度
	int height; //高度
	char *shape; //shape[0~width*height-1]表示飞机形状，判断需要，飞机大小不能小于2*2，且第2行第2列必须有字符
	int life; //生命值
	Type type;
	vector<Bullet*> bullets; //子弹
};

#endif // !PLANE_H
