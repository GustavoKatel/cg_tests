#include "target_rabbit.h"

#include "defines.h"

Target_Rabbit::Target_Rabbit():Target("models/Rabbit.obj", 20, 8)
{
	this->setScale(0.2,0.2,0.2);
};

Target_Rabbit::~Target_Rabbit(){};

