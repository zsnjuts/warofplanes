#include "Bullet.h"
#include <iostream>
using namespace std;

Bullet::Bullet()
{
}

Bullet::Bullet(int x, int y)
{
	this->x = x;
	this->y = y;
}

Bullet::Bullet(int x, int y, BulletDirection dir, int power, int speed): Bullet(x,y)
{
	this->dir = dir;
	this->power = power;
	this->state = READY;
	this->speed = speed;
}

void Bullet::delScreen(char ** screen)
{
	screen[1 + x][1 + y] = ' ';
}

void Bullet::synScreen(char ** screen)
{
	screen[1 + x][1 + y] = '.';
}

pair<int, int> Bullet::updatePosition()
{
	int newX = x;
	int newY = y;
	switch (dir)
	{
	case UP: newX = x - speed; break;
	case UPLEFT: newX = x - speed; newY = y - speed; break;
	case UPRIGHT: newX = x - speed; newY = y + speed; break;
	case DOWN: newX = x + speed; break;
	case DOWNLEFT: newX = x + speed; newY = y - speed; break;
	case DOWNRIGHT: newX = x + speed; newY = y + speed; break;
	default: break;
	}

	return make_pair(newX, newY);
}

bool Bullet::hit(char **screen)
{
/*	ofstream fout("log.txt", ios::app);
	fout << "hit @(" << x << "," << y << ")" << endl;
	fout.close();*/
	if (power <= 0)
		return false;
	else if (--power <= 0) //杀伤力在此次hit之后耗尽
	{
		delScreen(screen);
		return false;
	}
	else
		return true;
}
