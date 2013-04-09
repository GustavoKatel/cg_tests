#ifndef UTIL_H
#define UTIL_H

#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include <string>

namespace Util
{
	void Window2World(int x, int y, GLdouble *cx, GLdouble *cy, GLdouble *cz);
	void World2Window(float x, float y, float z, int *win_x, int *win_y);
	//
	void DrawText(int x, int y, std::string text, float r, float g, float b, void *size=GLUT_BITMAP_9_BY_15);
};


#endif
