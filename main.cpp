#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>

#include <GL/gl.h>
#include <GL/glut.h>

#include "defines.h"
#include "camera.h"
#include "controller.h"
#include "object.h"
#include "level.h"
#include "util.h"
#include "player.h"

int Player::bullets=2;

int help_window=0, about_window=0;
int info_win=0;

int cam_x=0, cam_y=0;

int level_index=-1;
std::vector<Level *> levels;

//-----------------------------------------------------------------------------
void update()
{
	glutWarpPointer(WIDTH/2,HEIGHT/2);
	//
	Controller::getController()->update();
	Camera::getCamera()->update();
	//
	if(levels.size()>=0 && level_index>=0)
	{
		Level *level = levels.at(level_index);
		switch(level->is_finished())
		{
			case 1:
				if(level_index==levels.size()-1)
					level_index=-1;
				else{
					level_index++;
					levels.at(level_index)->start();
				}
				break;
			case 2:
				level_index=-1;
				break;
		}
		if(level_index>=0)
		{
			Level *level = levels.at(level_index);
			level->update();
		}
	}
}

//-----------------------------------------------------------------------------
void display(void)
{
	update();
	//
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//
	float vw, vh;
	Camera::getCamera()->getViewport(&vw, &vh);
	//
	if(levels.size()>0 && level_index>=0)
	{
		levels.at(level_index)->draw();
		//
		std::ostringstream os;
		os<<Player::bullets<<" balas";
		Util::DrawText(0,1,os.str(), 1,0,0);
	}else if(info_win)
	{
		info_win--;
		if(help_window)
		{
			Util::DrawText(vw/2-30,vh/2,std::string("Criado por: GustavoBrito e EveraldoAndrade"), 1,1,0, GLUT_BITMAP_TIMES_ROMAN_24);
			
		}else if(about_window)
		{
			Util::DrawText(vw/2-30,vh/2,std::string("Para atirar apenas clique!"), 1,1,0, GLUT_BITMAP_TIMES_ROMAN_24);
			Util::DrawText(vw/2-30,vh/2-24,std::string("Acerte os alvos!"), 1,1,0, GLUT_BITMAP_TIMES_ROMAN_24);
			Util::DrawText(vw/2-30,vh/2-48,std::string("Divirta-se!"), 1,1,0, GLUT_BITMAP_TIMES_ROMAN_24);

		}
	}else{
		Util::DrawText(vw/2-30,vh/2,std::string("      [S]tart"), 1,1,0, GLUT_BITMAP_TIMES_ROMAN_24);
		Util::DrawText(vw/2-30,vh/2-24 ,std::string("     [H]elp"), 1,1,0, GLUT_BITMAP_TIMES_ROMAN_24);
		Util::DrawText(vw/2-30,vh/2-48 ,std::string(" Ab[O]ut"), 1,1,0, GLUT_BITMAP_TIMES_ROMAN_24);
		Util::DrawText(vw/2-30,vh/2-72 ,std::string("Exi[T]"), 1,1,0, GLUT_BITMAP_TIMES_ROMAN_24);
		//
		help_window=0;
		about_window=0;
	}
	//
	//
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

// Mapeia o teclado
void keyboard(unsigned char key, int x, int y)
{
	if(key==27)
		exit(0);
	if(levels.size()>0 && level_index>=0)
		levels.at(level_index)->command(key, x, y);
	else{
		switch(key)
		{
			case 's':
			case 'S':
				level_index++;
				levels.at(level_index)->start();
				levels.at(level_index)->command('s', x, y);
				break;
			case 't':
			case 'T':
				exit(1);
			case 'h':
			case 'H':
				help_window=1;
				info_win=60;
				break;
			case 'o':
			case 'O':
				about_window=1;
				info_win=60;
				break;
		}
	}
}
void keyboard_special(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			keyboard('w',x,y);
			break;
		case GLUT_KEY_DOWN:
			keyboard('s',x,y);
			break;
		case GLUT_KEY_LEFT:
			keyboard('a',x,y);
			break;
		case GLUT_KEY_RIGHT:
			keyboard('d',x,y);
			break;
	}
}
//-----------------------------------------------------------------------------
//mapeia o mouse
void mouse_move(int x, int y)
{
	if(levels.size()>0 && level_index>=0 && levels.at(level_index)->is_shooting())
		return;
	//
	float dx=0.5, dy=-0.5;
	int difx = x-cam_x;
	if(!difx)
		dx=0;
	else if(difx<0)
		dx=-dx;
	int dify = y-cam_y;
	if(!dify)
		dy=0;
	else if(dify>0)
		dy=-dy;
	//
	cam_x=x;
	cam_y=y;
	Camera::getCamera()->translate_look_angle(dy,dx);
}
// Mapeia o clique
void mouse_click(int button, int state,int x, int y)
{
	if(levels.size()>0 && level_index>=0)
		levels.at(level_index)->click(button, x, y);
	float vw, vh;
	Camera::getCamera()->getViewport(&vw, &vh);
	if(button==GLUT_RIGHT_BUTTON)
		std::cout<<"z-buffer: "<<Util::GetZat(vw/2,vh/2)<<std::endl;
}
//
void reshape(int w, int h) //callback de redimensionamento
{
	Camera::getCamera()->setViewport(w,h);
}
//-----------------------------------------------------------------------------
// Libera a memoria do objeto responsavel por guardar dados do modelo.
void FreeMemFunc(void)
{
	std::clog << "Exiting...\n";
	//
	unsigned int i;
	for(i=0;i<levels.size();i++)
		if(levels.at(i))
			delete levels.at(i);
	levels.clear();
	//
	std::map<std::string, objLoader*>::iterator it;
	for(it=Object::cache_objs.begin(); it!=Object::cache_objs.end(); ++it)
		delete it->second;
	Object::cache_objs.clear();
}

//-----------------------------------------------------------------------------
// Programa principal
int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(200,20);
	glutCreateWindow("CGPROJ");

//	glutFullScreen();

	glutWarpPointer(WIDTH/2,HEIGHT/2);

	glutDisplayFunc(display);

	//TODO fix tilt when starting to move
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	//
	//
	glutMouseFunc(mouse_click);
	glutPassiveMotionFunc(mouse_move);
	glutReshapeFunc(reshape);

	atexit(FreeMemFunc);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	//Camera
	Camera::getCamera()->setPos(0.0,0.5f,0.0f);
	Camera::getCamera()->setViewport(WIDTH, HEIGHT);
	Camera::getCamera()->update();
	//
	levels.push_back(new Level());
	levels.push_back(new Level(2));
	levels.push_back(new Level(3));
	//level_index=1;
	//
	GLfloat LightAmbient[]  = {0.1f, 0.1f, 0.1f};
	GLfloat LightDiffuse[]  = {1.0f, 1.0f, 1.0f};
	GLfloat LightSpecular[] = {1.0f, 1.0f, 1.0f};
	GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 0.0f }; 
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
	//
	GLfloat LightAmbient_1[]  = {0.1f, 0.1f, 0.1f};
	GLfloat LightDiffuse_1[]  = {1.0f, 1.0f, 1.0f};
	GLfloat LightSpecular_1[] = {1.0f, 1.0f, 1.0f};
	GLfloat LightPosition_1[] = {0.0f, 0.0f,-2.0f, 0.0f }; 
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient_1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse_1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular_1);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition_1);
	glEnable(GL_LIGHT1);

	//
	glEnable(GL_LIGHTING);
	//
	glutMainLoop();

	return 0;
}

