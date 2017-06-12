#include "MyPlane.h"
#include <iostream>
using namespace std;

MyPlane::MyPlane():Plane()
{
}

MyPlane::MyPlane(int x, int y):Plane(x,y)
{
}

MyPlane::MyPlane(int x, int y, int width, int height, const char * shape):
	Plane(x,y,width,height,shape)
{
}

MyPlane::MyPlane(int x, int y, int width, int height, const char * shape, int life):
	Plane(x, y, width, height, shape, life, MYPLANE)
{
}


