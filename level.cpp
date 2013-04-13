#include "level.h"

#include <cstdlib>

#include "controller.h"
#include "camera.h"
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
	is_shooting=0;
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
	if(is_shooting)
		return;
	Controller::getController()->command(key, x, y);
}

void Level::click(int button, int x, int y)
{
	if(button==0)
	{
		is_shooting=1;
	}
}

void Level::update()
{
	if(is_shooting)
	{
		Camera::getCamera()->translate(0.0f, 0.0f,PLAYER_STEP*SHOT_VELOCITY);
	}
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
	params[0] = 0.1;
	params[1] = 0.1;
	params[2] = 0.1;
	glMaterialfv(GL_FRONT, GL_AMBIENT, params);
	//diffuse
	params[0] = 0.1;
	params[1] = 1.0;
	params[2] = 0.1;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, params);
	//specular
	params[0] = 0.1;
	params[1] = 0.1;
	params[2] = 0.1;
	glMaterialfv(GL_FRONT, GL_SPECULAR, params);
	//
	glMaterialf(GL_FRONT, GL_SHININESS, 128);
	glColor3f(0.0f,1.0f,0.0f);
	//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float angle_y, angle_x;
	Camera::getCamera()->getLookAngle(&angle_y, &angle_x);
	float cy;
	Camera::getCamera()->getPos(NULL, &cy, NULL);
	//
	//camera translate (only y)
	glTranslatef(0.0f, -cy, 0.0f);
	//camera rotate
	glRotatef(angle_y, 0,1,0);
	glRotatef(angle_x, 1,0,0);
	//
	glBegin(GL_QUADS);
	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f(-15, 0.0,-15);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f(-15, 0.0, 15);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f( 15, 0.0, 15);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f( 15, 0.0,-15);
	glEnd();

	//
	unsigned int i;
	for(i=0;i<targets.size();i++)
		targets.at(i)->draw();
	//
	for(i=0;i<trees.size();i++)
		trees.at(i)->draw();
}

//

void Level::initTargets()
{
	int i;
	for(i=0;i<this->difficulty;i++)
	{
		Target *t;
		if(difficulty==1)
			t = new Target_Rabbit();
		if(difficulty==2)
			t = new Target_Deer();
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
