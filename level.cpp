#include "level.h"

#include <cstdlib>
#include <string>
#include <sstream>

#include <GL/gl.h>
#include <GL/glut.h>

#include "controller.h"
#include "camera.h"
#include "util.h"
#include "defines.h"
#include "player.h"

#include "target_deer.h"
#include "target_tiger.h"

Level::Level(int difficulty)
{
	loading=0;
	loading_message = std::string("Level #! Loading!");
	_is_finished=0;
	//
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
	gun->addAngle(20,1,0,0);
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

void Level::start()
{
	_is_finished=0;
	_is_shooting=0;
	//
	int total_life=0;	
	for(unsigned int i=0;i<targets.size();i++)
	{
		delete targets.at(i);
	}
	targets.clear(); initTargets();
	for(unsigned int i=0;i<targets.size();i++)
	{
		total_life+=targets.at(i)->getLife();
	}
	Player::bullets=total_life/20;
	std::cout<<"bullets: "<<Player::bullets<<" life: "<<total_life<<std::endl;
}

void Level::command(unsigned char key, int x, int y)
{
/*	float px=0.0, py=0.0, pz=0.0;
	Camera::getCamera()->getPos(&px, &py, &pz);
	if(!_is_shooting && check_collision() && key=='w')
	{
		std::cout<<"Don't move!"<<std::endl;
		return;
	}*/
	Controller::getController()->command(key, x, y);
}

void Level::click(int button, int x, int y)
{
	if(button==0 && !_is_shooting && Player::bullets>0 && !loading)
	{
		_is_shooting=1;
		Player::bullets--;
	}
}

int Level::is_shooting()
{
	return _is_shooting;
}

void Level::update()
{
	if(loading)
	{
		loading--;
		return;
	}
	float cx, cy, cz;
	Camera::getCamera()->getPos(&cx, &cy, &cz);
	float ay;
	Camera::getCamera()->getLookAngle(&ay, NULL);
	float vw, vh;
	Camera::getCamera()->getViewport(&vw, &vh);
	if(_is_shooting)
	{
		Camera::getCamera()->translate(0.0f, 0.0f,PLAYER_STEP*SHOT_VELOCITY);
		Camera::getCamera()->getPos(&cx, &cy, &cz);
	}else{
		gun->setPos(cx, cy, cz);
	}
	bullet->setPos(cx, cy-2, cz+2);
	glutWarpPointer(vw/2, vh/2);//+vh/4);

	for(unsigned int i=0;i<targets.size();i++)
	{
		targets.at(i)->update();
	}
	//
	if(check_collision())
	{
		unsigned int i;
		for(i=0;i<targets.size();i++)
		{
			if(targets.at(i)->getLife()>0)
				break;
		}
		if(i==targets.size())
		{
			loading_message=std::string("YOU WIN!");
			end_game(1);
		}
	}
	//
	if(!Player::bullets && !_is_shooting)
	{
		int total = 0;
		for(unsigned int i=0;i<targets.size();i++)
		{
			total+=targets.at(i)->getLife();
		}
		if(total)
			end_game(2);
	}
}

int Level::is_finished()
{
	return _is_finished;
}

void Level::end_game(int code)
{
	loading=60;
	_is_finished=code;
}

void Level::draw()
{
	int pontos_total=0;
	float vw, vh;
	Camera::getCamera()->getViewport(&vw, &vh);
	if(loading)
	{
		Util::DrawText(vw/2-30, vh/2, loading_message, 1, 0, 0);
	}	
	//
	//floor
	GLfloat params[4];
	params[0] = 0.5;
	params[1] = 0.5;
	params[2] = 0.5;
	params[3] = 1.0;
	//ambient
	params[0] = 0.1;
	params[1] = 1.0;
	params[2] = 0.1;
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
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
	glColor3f(0.0f,1.0f,0.0f);
	//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float angle_x, angle_y;
	Camera::getCamera()->getLookAngle(&angle_x, &angle_y);
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
		pontos_total+=targets.at(i)->getLife();
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
	std::ostringstream os;
	os<<"Pontos de vida: "<<pontos_total;
	Util::DrawText(0,vh-48, os.str(), 1,0,0);
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
			t = new Target_Deer();
		if(difficulty==3)
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

int Level::check_collision()
{
	float vw, vh;
	Camera::getCamera()->getViewport(&vw, &vh);
	float cx, cy, cz;
	Camera::getCamera()->getPos(&cx, &cy, &cz);
	float zbuff = Util::GetZat(vw/2,vh/2);
	int res=0;
	if(zbuff<=0.959400)
	{
		Target *t = this->get_target_touched();
		if(t)
		{
			if(_is_shooting)
			{
				t->shot();
				loading_message = std::string("GREAT JOB!");
				loading=60;
				_is_shooting=0;
				Camera::getCamera()->setPos(0.0,0.5f,0.0f);
			}
		}else{
			if(_is_shooting)
			{
				_is_shooting=0;
				Camera::getCamera()->setPos(0.0,0.5f,0.0f);
				loading_message = std::string("OPS!");
				loading=60;
				
			}
		}
		res=1;
	}else if(zbuff>0.99 && _is_shooting && (cz>15||cz<-15) )
	{
		if(!Player::bullets)
		{
			loading_message=std::string("YOU LOSE!");
			end_game(2);
		}
		_is_shooting=0;
		Camera::getCamera()->setPos(0.0,0.5f,0.0f);
		res=0;
	}
	return res;
}

Target *Level::get_target_touched()
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
				return targets.at(j);
			}
	}

 	glMatrixMode(GL_MODELVIEW);
	return NULL;
}
