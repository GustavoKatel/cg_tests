#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "camera.h"

typedef enum {w=0, a, s, d, q, e} COMMANDS;

class Controller
{
public:
	static Controller *getController();
	static void destroy();
	//
	void update();
	//
	void command(char c, int x, int y);
protected:
	static Controller *controller;
	//
	Controller();
	virtual ~Controller();
private:
	unsigned int command_buff[6]; 

};


#endif
