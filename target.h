#ifndef TARGET_H
#define TARGET_H

#include "object.h"


class Target : public Object
{
public:
	Target(const char *model_file_name="", int life=60, int velocity=10);
	virtual ~Target();
	
	virtual int is_touched(float x, float y, float z, int shoting=0);
	virtual void shot();

	virtual void die();
	
	virtual void draw();

protected:
	int life;
	int velocity;	


};

#endif

