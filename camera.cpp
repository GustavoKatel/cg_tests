#include "camera.h"

#include <iostream>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>

Camera *Camera::camera = NULL;

Camera *Camera::getCamera()
{
	if(!camera)
		camera = new Camera();
	return camera;
}

Camera::Camera()
{
	pos_x = pos_y = pos_z = 0.0;
	look_x = look_y = look_z = 0.0;
	up_x = up_z = 0.0; up_y = 1.0;
}

Camera::~Camera()
{
}

void Camera::setPos(float px, float py, float pz)
{
	pos_x = px;
	pos_y = py;
	pos_z = pz;
}
void Camera::getPos(float *px, float *py, float *pz)
{
	if(px) *px = pos_x;
	if(py) *py = pos_y;
	if(pz) *pz = pos_z;
}

void Camera::setLook(float lx, float ly, float lz)
{
	look_x = lx;
	look_y = ly;
	look_z = lz;
}
void Camera::getLook(float *lx, float *ly, float *lz)
{
	if(lx) *lx = look_x;
	if(ly) *ly = look_y;
	if(lz) *lz = look_z;
}
//
void Camera::setUp(float ux, float uy, float uz)
{
	up_x = ux;
	up_y = uy;
	up_z = uz;
}
void Camera::getUp(float *ux, float *uy, float *uz)
{
	if(ux) *ux = up_x;
	if(uy) *uy = up_y;
	if(uz) *uz = up_z;
}
//
void Camera::setViewport(float width, float height)
{
	viewport_w = width;
	viewport_h = height;
}
void Camera::getViewport(float *w, float *h)
{
	if(w) *w = viewport_w;
	if(h) *h = viewport_h;
}
//
void Camera::translate(float dx, float dy, float dz)
{
	pos_x = inc(pos_x,dx,1,0,0);
	pos_y = inc(pos_y,dy,0,1,0);
	pos_z = inc(pos_z,dz,0,0,1);
	//
	look_x = inc(look_x,dx,1,0,0);
	look_y = inc(look_y,dy,0,1,0);
	look_z = inc(look_z,dz,0,0,1);
}

void translate_look(float ldx, float ldy)
{
	look_x = inc(look_x,ldx,1,0,0);
	look_y = inc(look_y,ldy,0,1,0);
}

float Camera::inc(float v1, float v2, float x, float y, float z)
{
	if( (1+look_z/1+abs(look_z)) - (1+pos_z/1+abs(pos_z)) >=0)
	{
		return v1-v2;
	}else{
		return v1+v2;
	}
}

void Camera::update()
{
	glViewport(0,0, viewport_w, viewport_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(55.0f,
			viewport_w/viewport_h,
			0.05f,
			150f);
	gluLookAt(pos_x,pos_y,pos_z,
		look_x,look_y,look_z,
		up_x,up_y,up_z);
}
