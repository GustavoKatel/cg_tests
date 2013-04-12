#include "level.h"

#include <cstdlib>

Level::Level(int difficulty)
{
	this->difficulty = difficulty;
	//
	initTargets();
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

void Level::update()
{

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
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f(-10000,0.0,-10000);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f(-10000,0.0, 10000);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f( 10000,0.0,-10000);
	//
	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f( 10000,0.0,-10000);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f( 10000,0.0, 10000);

	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3f(-10000,0.0, 10000);
	glEnd();

	//
	unsigned int i;
	for(i=0;i<targets.size();i++)
		targets.at(i)->draw();
}

//

void Level::initTargets()
{
	int i;
	for(i=0;i<this->difficulty;i++)
	{
		targets.push_back(new Object("models/Deer.obj"));
		targets.at(i)->setPos( (rand()%1 ? -1 : 1)*rand()%10,
				0.0,
				-rand()%20);
		targets.at(i)->setScale(0.2,0.2,0.2);
	}
}
