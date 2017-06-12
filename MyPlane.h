#ifndef MYPLANE_H
#define MYPLANE_H

#include "Plane.h"

class MyPlane:protected Plane
{
	friend class Control;
public:
	MyPlane();
	MyPlane(int x, int y);
	MyPlane(int x, int y, int width, int height, const char *shape);
	MyPlane(int x, int y, int width, int height, const char *shape, int life);

private:
	
};

#endif // !MYPLANE_H

