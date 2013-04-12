#include "camera.h"

#include <iostream>
#include <math.h>

#include <GL/glut.h>
#include <GL/glu.h>
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
	angle_x = angle_y = 0.0;
}

Camera::~Camera()
{
	delete camera;
	camera=NULL;
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

void Camera::setLookAngle(float lax, float lay)
{
	this->angle_x = lax;
	this->angle_y = lay;
}
void Camera::getLookAngle(float *lax, float *lay)
{
	if(lax) *lax = this->angle_x;
	if(lay) *lay = this->angle_y;
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
	if(look_z-pos_z>=0)
	{
		pos_x-=dx;
		look_x-=dx;
		//
		pos_y+=dy;
		look_y+=dy;
		//
		pos_z+=dz;
		look_z+=dz;
	}
	else
	{
		pos_x+=dx;
		look_x+=dx;
		//
		pos_y-=dy;
		look_y-=dy;
		//
		pos_z-=dz;
		look_z-=dz;
		//std::cout<<"loo_z: "<<look_z<<" pos_z: "<<pos_z<<std::endl;
	}
}

void Camera::translate_look(float ldx, float ldy, float ldz)
{
	if(look_z-pos_z>=0)
	{
		look_x-=ldx;
		//
		look_y+=ldy;
		//
		look_z+=ldz;
	}
	else
	{
		look_x+=ldx;
		look_y-=ldy;
		look_z-=ldz;
	}
}

void Camera::translate_look_angle(float ldx, float ldy)
{
	this->angle_x+=ldx;
	if(angle_x>360)
		angle_x=angle_x-360;
	if(angle_x<0)
		angle_x=angle_x+360;

	this->angle_y+=ldy;
	if(angle_y>360)
		angle_y=angle_y-360;
	if(angle_y<0)
		angle_y=angle_y+360;
}

float Camera::inc(float v1, float v2, float x, float y, float z)
{
	/*if( (1+look_z/1+abs(look_z)) - (1+pos_z/1+abs(pos_z)) >=0)
	{
		return v1-v2;
	}else{
		return v1+v2;
	}*/
	if(look_z-pos_z>=0)
	{
		if(x)
			return v1-v2;
		else
			return v1+v2;
	}else{
		if(x)
			return v1+v2;
		else
			return v1-v2;
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
			150.0f);
	/*gluLookAt(pos_x,pos_y,pos_z,
		look_x,look_y,look_z,
		up_x,up_y,up_z);*/
	gluLookAt(0.0f, 0.0f, 0.0f,
		0.0f,0.0f,-1.0f,
		0.0f, 1.0f, 0.0f);
}
