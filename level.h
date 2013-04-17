#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <vector>

#include <GL/gl.h>

#include "object.h"
#include "camera.h"
#include "target.h"

class Level
{

public:
	Level(int difficulty=1);
	~Level();
	//
	void start();
	//
	void command(unsigned char key, int x, int y);
	void click(int button, int x, int y);
	//
	int is_shooting();
	//
	void update();
	void draw();
	//
	int is_finished();
private:
	int loading;
	std::string loading_message;
	//
	void initTargets();
	void initTrees();
	//
	void end_game(int code=1);
	int _is_finished;
	//
	Target *get_target_touched();
	int check_collision();
	//
	int difficulty;
	std::vector<Target *> targets;
	std::vector<Object *> trees;
	//
	Object *bullet;
	Object *gun;
	//
	int _is_shooting;

};

#endif
