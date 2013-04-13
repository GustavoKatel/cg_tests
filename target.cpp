#include "target.h"

Target::Target(const char *model_file_name, int life, int velocity):Object(model_file_name)
{
	this->life=life;
	this->velocity=velocity;
}

Target::~Target()
{
}

int Target::is_touched(float x, float y, float z, int shoting)
{
	if(Object::is_touched(x, y, z))
	{
		shot();
		return 1;
	}
	return 0;	
}

void Target::shot()
{
	life-=20;
	velocity-=1;
}

void Target::die()
{

}

void Target::draw()
{
	Object::draw();
}
