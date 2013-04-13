#include "controller.h"

#include <iostream>

#include "defines.h"

Controller *Controller::controller = NULL;

Controller *Controller::getController()
{
	if(!controller)
		controller = new Controller();
	return controller;
}

void Controller::update()
{
	if(command_buff[w])
		Camera::getCamera()->translate(0.0f, 0.0f, PLAYER_STEP);
	
	if(command_buff[s])	
		Camera::getCamera()->translate(0.0f, 0.0f,-PLAYER_STEP);
	
	if(command_buff[d])		
		Camera::getCamera()->translate(PLAYER_STEP,0.0f,0.0f);
			
	if(command_buff[a])
		Camera::getCamera()->translate(-PLAYER_STEP,0.0f,0.0f);
			
	if(command_buff[q])
		Camera::getCamera()->translate_look_angle(-0.5,0.0f);

	if(command_buff[e])		
		Camera::getCamera()->translate_look_angle( 0.5,0.0f); 
}

void Controller::command(char c, int x, int y)
{
	switch(c)
	{
		case 'w':
			command_buff[w]=!command_buff[w];
			break;
		case 's':
			command_buff[s]=!command_buff[s];
			break;
		case 'a':
			command_buff[a]=!command_buff[a];
			break;
		case 'd':
			command_buff[d]=!command_buff[d];
			break;
		case 'q':
			command_buff[q]=!command_buff[q];
			break;
		case 'e':
			command_buff[e]=!command_buff[e];
			break;
	}
}

//
Controller::Controller()
{
	int i;
	for(i=0;i<6;i++)
		command_buff[i]=0;
}

Controller::~Controller()
{
	delete controller;
	controller=NULL;
}
