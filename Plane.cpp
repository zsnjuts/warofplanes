#include "Plane.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

static int defaultWidth = 3;
static int defaultHeight = 3;
static char defaultShape[] = " - -+- | ";

Plane::Plane()
{
	x = y = 0;
	width = defaultWidth;
	height = defaultHeight;
	life = 1;
	shape = new char[defaultWidth*defaultHeight];
	memcpy(shape, defaultShape, defaultWidth*defaultHeight);
}

Plane::Plane(int x, int y)
{
	Plane();
	this->x = x;
	this->y = y;
}

Plane::Plane(int x, int y, int width, int height, const char * shape)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	life = 1;
	this->shape = new char[width*height];
	memcpy(this->shape, shape, width*height);
}

Plane::Plane(int x, int y, int width, int height, const char * shape, char ** screen):
	Plane(x, y, width, height, shape)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (shape[i*width + j] != ' ')
				screen[1 + x + i][1 + y + j] = shape[i*width + j];
}

Plane::Plane(int x, int y, int width, int height, const char * shape, int life) :
	Plane(x, y, width, height, shape)
{
	this->life = life;
}

Plane::Plane(int x, int y, int width, int height, const char * shape, int life, Type type):
	Plane(x, y, width, height, shape, life)
{
	this->type = type;
}

Plane::Plane(int x, int y, int width, int height, const char * shape, char ** screen, int life) :
	Plane(x, y, width, height, shape, screen)
{
	this->life = life;
}

Plane::Plane(int x, int y, int width, int height, const char * shape, char ** screen, int life, Type type):
	Plane(x, y, width, height, shape, screen, life)
{
	this->type = type;
}

void Plane::delScreen(char ** screen)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (shape[i*width + j] != ' ')
				screen[1 + x + i][1 + y + j] = ' ';
}

void Plane::synScreen(char **screen)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (shape[i*width + j] != ' ')
				screen[1 + x + i][1 + y + j] = shape[i*width + j];
}

bool Plane::crash(char **screen)
{
/*	ofstream fout("log.txt", ios::app);
	fout << "crash @(" << x << "," << y << ")" << endl;
	fout.close();*/
	if (life <= 0) //若已经耗尽生命值
		return false;
	else if (--life <= 0) //若生命值此次crash后耗尽
	{
		delScreen(screen);
		return false;
	}
	else
		return true;
}

Plane::~Plane()
{
	delete[]shape;
}
