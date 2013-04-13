#include <iostream>
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

float shot_x=0.0;
float shot_y=0.0;
float shot_z=0.0;

float ds_x=0.0;
float ds_y=0.0;
float ds_z=0.0;

int cam_x=0, cam_y=0;

//Object *obj=NULL;
unsigned int level_index=0;
std::vector<Level *> levels;

//-----------------------------------------------------------------------------
void update()
{
	glutWarpPointer(WIDTH/2,HEIGHT/2);
	//
	Controller::getController()->update();
	Camera::getCamera()->update();
	//
	if(levels.size()>=0)
		levels.at(level_index)->update();
}

//-----------------------------------------------------------------------------
void display(void)
{
	update();
	//
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//
	levels.at(level_index)->draw();
	//
	Util::DrawText(0,0,std::string("100/5"), 1,1,0, GLUT_BITMAP_TIMES_ROMAN_24);
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
	if(levels.size()>0)
		levels.at(level_index)->command(key, x, y);
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
//mapeia o mous
void mouse_move(int x, int y)
{
	float dx=0.08, dy=-0.08;
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
/*	if(abs(difx)<2)
		dx=0;
	if(abs(dify)<2)
		dy=0;*/
	//
	cam_x=x;
	cam_y=y;
	Camera::getCamera()->translate_look_angle(dx,dy);
	//std::cout<<"difx: "<<difx<<" dify: "<<dify<<std::endl;
	//Camera::getCamera()->setLook(wx, wy, wz);
}
// Mapeia o clique
void mouse_click(int button, int state,int x, int y)
{
	if(levels.size()>0)
		levels.at(level_index)->click(button, x, y);
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

	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	//Camera
	Camera::getCamera()->setPos(0.0,0.08f,0.0f);
	Camera::getCamera()->setLook(0.0,0.0,-1.0);
	Camera::getCamera()->setViewport(WIDTH, HEIGHT);
	Camera::getCamera()->update();
	//
	levels.push_back(new Level());
	//
	GLfloat LightAmbient[]  = {0.1f, 0.1f, 0.1f};
	GLfloat LightDiffuse[]  = {1.0f, 1.0f, 1.0f};
	GLfloat LightSpecular[] = {1.0f, 1.0f, 1.0f};
	GLfloat LightPosition[] = {0.0f, 0.0f,-2.0f, 0.0f }; 
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
	//
	glutMainLoop();

	return 0;
}

