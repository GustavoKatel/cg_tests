#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <vector>

#include <GL/gl.h>

#include "object.h"
#include "camera.h"

class Level
{

public:
	Level(int difficulty=1);
	~Level();
	//
	void update();
	void draw();
private:
	void initTargets();
	//
	int difficulty;
	std::vector<Object *> targets;
	std::vector<Object *> trees;

};

#endif
