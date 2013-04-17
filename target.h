#ifndef TARGET_H
#define TARGET_H

#include "object.h"

#include "defines.h"

class Target : public Object
{
public:
	Target(const char *model_file_name="", int life=40, float velocity_t=5);
	virtual ~Target();
	
	virtual void shot();

	virtual int getLife();

	virtual void update();
	virtual void draw(int boundingBox=0);

protected:
	int life;
	float velocity;	
	//
	float tdx, tdy, tdz;
	float tx, ty, tz;
	//
	virtual void newPositionTarget();


};

#endif

