#include "level.h"

#include <cstdlib>

#include <GL/gl.h>
#include <GL/glut.h>

#include "controller.h"
#include "camera.h"
#include "util.h"
#include "defines.h"

#include "target_deer.h"
#include "target_rabbit.h"

#define check_touch(x,y,z) for(unsigned int i=0;i<targets.size();i++) if(targets.at(i)->is_touched(x,y,z)) { std::cout<<"touch!\n"; return; }

Level::Level(int difficulty)
{
	this->difficulty = difficulty;
	//
	initTargets();
	initTrees();
	//
	_is_shooting=0;
	//
	bullet = new Object("models/9bulllet.obj");
	bullet->setScale(0.003,0.003,0.003);
	bullet->addAngle(90, 1,0,0);
	gun = new Object("models/L11A3.obj");
}

Level::~Level()
{
	for(unsigned int i=0;i<targets.size();i++)
		delete targets.at(i);
	targets.clear();
	//
	for(unsigned int i=0;i<trees.size();i++)
		delete trees.at(i);
	trees.clear();
	//
	delete gun;
	delete bullet;
}

void Level::command(unsigned char key, int x, int y)
{
	float px=0.0, py=0.0, pz=0.0;
	Camera::getCamera()->getPos(&px, &py, &pz);
	switch(key)
	{
		case 'w':
			check_touch(-px, -py, -(pz+PLAYER_STEP));
			break;
		case 's':
			check_touch(-px, -py, -(pz-PLAYER_STEP));
			break;
		case 'd':
			check_touch(-(px+PLAYER_STEP), -py, -pz);
			break;
		case 'a':
			check_touch(-(px-PLAYER_STEP), -py, -pz);
			break;

	}
	if(_is_shooting)
		return;
	Controller::getController()->command(key, x, y);
}

void Level::click(int button, int x, int y)
{
	if(button==0)
	{
		_is_shooting=1;
	}
}

int Level::is_shooting()
{
	return _is_shooting;
}

void Level::update()
{
	float cx, cy, cz;
	Camera::getCamera()->getPos(&cx, &cy, &cz);
	float ay;
	Camera::getCamera()->getLookAngle(&ay, NULL);
	if(_is_shooting)
	{
		Camera::getCamera()->translate(0.0f, 0.0f,PLAYER_STEP*SHOT_VELOCITY);
		Camera::getCamera()->getPos(&cx, &cy, &cz);
	}else{
		gun->setPos(cx, cy, cz);
	}
	bullet->setPos(cx, cy-2, cz+2);

	for(int i=0;i<targets.size();i++)
	{
		targets.at(i)->update();
	}
	//
	//check_collision();
}

void Level::draw()
{
	//floor
	GLfloat params[4];
	params[0] = 0.5;
	params[1] = 0.5;
	params[2] = 0.5;
	params[3] = 1.0;
	//ambient
	params[0] = 1.0;
	params[1] = 1.0;
	params[2] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, params);
	//diffuse
	params[0] = 0.1;
	params[1] = 1.0;
	params[2] = 0.1;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, params);
	//specular
	params[0] = 0.0;
	params[1] = 0.0;
	params[2] = 0.0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, params);
	//
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
	glColor3f(0.0f,1.0f,0.0f);
	//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float angle_y, angle_x;
	Camera::getCamera()->getLookAngle(&angle_y, &angle_x);
	float cx, cy, cz;
	Camera::getCamera()->getPos(&cx, &cy, &cz);
	//
	//camera translate (only y)
	glTranslatef(0.0f,-1.0f/*-cy*/, 0.0f);
	//camera rotate
//	glRotatef(angle_y, 0,1,0);
	glRotatef(angle_x, 1,0,0);
	//
	glBegin(GL_QUADS);
	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f(-1500, 0.0,-1500);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f(-1500, 0.0, 1500);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f( 1500, 0.0, 1500);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f( 1500, 0.0,-1500);
	glEnd();

	//
	unsigned int i;
	for(i=0;i<targets.size();i++)
	{
		targets.at(i)->draw();
	}
	//
	for(i=0;i<trees.size();i++)
		trees.at(i)->draw();
	//
	if(_is_shooting)
	{
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(0,-0.5, -2.0);
		bullet->draw(0,1);
		glPopMatrix();
	}
	if(!_is_shooting)
	{
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(0,-0.5, -0.5);
		gun->draw(0,1);
		glPopMatrix();
	}
}

//

void Level::initTargets()
{
	int i;
	for(i=0;i<this->difficulty;i++)
	{
		Target *t;
		if(difficulty==1)
			t = new Target_Deer();
		if(difficulty==2)
			t = new Target_Rabbit();
		targets.push_back(t);
		targets.at(i)->setPos( (rand()%1 ? -1 : 1)*rand()%10,
				0.0,
				-rand()%10);
	}
}

void Level::initTrees()
{
	unsigned int i;
	for(i=0;i<5;i++)
	{
		Object *tree = new Object("models/tree_oak.obj");
		tree->setPos((rand()%1 ? -1 : 1)*rand()%10,
				0.0,
				-rand()%10);
		tree->setScale(0.5, 0.5, 0.5);
		trees.push_back(tree);
	}
}

void Level::check_collision()
{
	float vw, vh;
	Camera::getCamera()->getViewport(&vw, &vh);
	float cx, cy, cz;
	Camera::getCamera()->getPos(&cx, &cy, &cz);

 	GLuint buff[64];
 	GLint touches, view[4];
 
 	glSelectBuffer(64, buff);
 
 	glGetIntegerv(GL_VIEWPORT, view);
 
 	glRenderMode(GL_SELECT);
 
 	glInitNames();
 
 	glPushName(0);
 
 	glMatrixMode(GL_PROJECTION);
 	glPushMatrix();
 		glLoadIdentity();
 
 		gluPickMatrix(vw/2, vh/2, 1.0, 1.0, view);
 		gluPerspective(55.0f, vw/vh, 0.05f, 150.0f);
 
 		glMatrixMode(GL_MODELVIEW);
 
 		glutSwapBuffers();

 		this->draw();
 
 		glMatrixMode(GL_PROJECTION);
 	glPopMatrix();
 
 	touches = glRenderMode(GL_RENDER);
 
 	int i;
 	for (i=0;i<touches;i++)
	{
		int tid = (GLubyte)buff[i * 4 + 3];
		int j;
		for(j=0;j<targets.size();j++)
			if(targets.at(j)->getId()==tid)
			{
				std::cout<<"z-buffer: "<<Util::GetZat(vw/2,vh/2)<<" cz: "<<cz<<std::endl;
				//targets.at(j)->die();
			}
	}

 	glMatrixMode(GL_MODELVIEW);
}
