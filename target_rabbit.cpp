#include "target_rabbit.h"

#include "defines.h"

Target_Rabbit::Target_Rabbit():Target("models/Rabbit.obj", 20, 8)
{
	this->setScale(0.08,0.08,0.08);
};

Target_Rabbit::~Target_Rabbit(){};

