#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <stdio.h>

#include "defines.h"
#include "object.h"

#define CAM_STEPS 80

float shot_x=0.0;
float shot_y=0.0;
float shot_z=0.0;

float ds_x=0.0;
float ds_y=0.0;
float ds_z=0.0;

Object *obj=NULL;

//-----------------------------------------------------------------------------
void update()
{

}

//-----------------------------------------------------------------------------
void display(void)
{
	update();
	//
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//
	obj->draw();
	//

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

// Mapeia o teclado
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
			exit(0);
			break;
	}
}
void keyboard_special(int key, int x, int y)
{
}
//-----------------------------------------------------------------------------
//mapeia o mous
void mouse_move(int x, int y)
{
}
// Mapeia o clique
void mouse_click(int button, int state,int x, int y)
{
}
//
void reshape(int w, int h) //callback de redimensionamento
{
}
//-----------------------------------------------------------------------------
// Libera a memoria do objeto responsavel por guardar dados do modelo.
void FreeMemFunc(void)
{
	std::clog << "Exiting...\n";
	//
	if(obj)
		delete obj;
}

//-----------------------------------------------------------------------------
// Programa principal
int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGTH);
	glutInitWindowPosition(100,20);
	glutCreateWindow("CGPROJ");

	glutDisplayFunc(display);

	//TODO fix tilt when starting to move
	//	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	//
	//
	glutMouseFunc(mouse_click);
	//	glutPassiveMotionFunc(mouse_move);
	glutReshapeFunc(reshape);

	atexit(FreeMemFunc);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	//
	obj = new Object("models/tree_oak.obj");
	obj->setPos(0,0,0);
	obj->setScale(0.2,0.2,0.2);
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

