#ifndef MYPLANE_H
#define MYPLANE_H

#include "Plane.h"

class MyPlane:protected Plane
{
	friend class Control;
public:
	MyPlane();
    MyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life);
	
};

#endif // !MYPLANE_H

