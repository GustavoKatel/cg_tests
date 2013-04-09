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
		targets.at(i)->setPos( (rand()%1 ? -1 : 1)*rand()%20,
				0.0,
				-rand()%20);
		targets.at(i)->setScale(0.2,0.2,0.2);
	}
}
