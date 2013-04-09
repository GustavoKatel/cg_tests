#include "util.h"

#include <GL/gl.h>
#include <GL/glut.h>

#include "defines.h"
#include "camera.h"

void Util::Window2World(int x, int y, GLdouble *cx, GLdouble *cy, GLdouble *cz)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	if(cx) *cx = posX;
	if(cy) *cy = posY;
	if(cz) *cz = posZ;
}

void Util::World2Window(float x, float y, float z, int *win_x, int *win_y)
{

	/*GLdouble cx = 0; //( x * 2/WIDTH  ) - 1 ;
	  GLdouble cy = 0; //( y * 2/HEIGTH ) - 1 ;
	  GLdouble cz = 0;*/
	//
	GLdouble cx, cy, cz;
	//
	GLint view[4];
	GLdouble model[16], proj[16];
	//
	glGetIntegerv(GL_VIEWPORT, view);
	glGetDoublev(GL_MODELVIEW_MATRIX, model);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	//
	gluProject(x,  y,  z,  model,  proj,  view,  (GLdouble *)&cx,  (GLdouble *)&cy,  (GLdouble *)&cz);
	//
	//cy = view[3] + (GLint) cy + 1;
	//
	*win_x = (int)cx;
	*win_y = (int)cy;
}

void Util::DrawText(int x, int y, std::string text, float r, float g, float b, void *size)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//
	float w, h;
	Camera::getCamera()->getViewport(&w, &h);
	glOrtho(0, w, 0, h, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*gluLookAt(	0.0f, 0.0f, 0.0f, 
				0.0f, 0.0f,-1.0f, 
				0.0f, 1.0f, 0.0f);*/

	glColor3f(r, g, b); // Green (text color)

	glRasterPos2i(x, y); // texture magnification text

	for(unsigned int i=0;i<text.size();i++)
	{
		char c = text[i];
		glutBitmapCharacter(size, c);  
	}
	//
	Camera::getCamera()->update();
}

