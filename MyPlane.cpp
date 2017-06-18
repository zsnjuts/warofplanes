#include "MyPlane.h"
#include <iostream>
using namespace std;

MyPlane::MyPlane():Plane()
{
}

MyPlane::MyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life):
    Plane(x, y, imageFile, scene, life, ME)
{
    skill = 0;
}
