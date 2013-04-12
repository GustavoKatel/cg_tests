#ifndef BBOX_H
#define BBOX_H

#include <GL/gl.h>
#include <GL/glut.h>

#include "objLoader.h"

class BoudingBox
{

public:
	BoudingBox(objLoader *objData);
	~BoudingBox();
	//
	int is_touched(float x, float y, float z);
	//
	void setScale(float x, float y, float z);
	//
	void draw();
private:
	float bx1, by1, bz1;
	float bx2, by2, bz2;
	//
	float sx, sy, sz;

};


#endif
