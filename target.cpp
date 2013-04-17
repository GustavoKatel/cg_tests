#include "target.h"

#include <iostream>
#include <cstdlib>

#include "defines.h"

Target::Target(const char *model_file_name, int life, float velocity_t):Object(model_file_name)
{
	this->life=life;
	this->velocity=velocity_t*PLAYER_STEP*0.01;
	std::cout<<"v: "<<velocity<<std::endl;
	//
}

Target::~Target()
{
}

void Target::shot()
{
	life-=20;
	velocity-=PLAYER_STEP*0.01;
}

int Target::getLife()
{
	return life;
}

void Target::update()
{
	Object::update();
	//
	float d = velocity;
	float px = pos_x+tdx*d, py = pos_y, pz = pos_z+tdz*d;
	this->setPos(px, py, pz);
	//
	if(px==tx || py==ty || pz==tz)
		this->newPositionTarget();
}

void Target::draw(int boundingBox)
{
	if(life)
		Object::draw(boundingBox);
}

void Target::newPositionTarget()
{
	tx = (rand()%1 ? -1 : 1)*rand()%10;
	if(tx-pos_x<0) tdx=-1; else tdx=1;
	ty = 0.0; tdy=0;
	tz = (rand()%1 ? -1 : 1)*rand()%10;
	if(tz-pos_z<0) tdz=-1; else tdz=1;
}
