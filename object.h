#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string.h>
#include <vector>
#include <GL/gl.h>

#include "SOIL.h"
#include "objLoader.h"

#include "bbox.h"

class Object
{
public:
	Object(const char *model_file_name="");
	virtual ~Object();
	//
	void loadTextures();
	//
	void getPos(float *x, float *y, float *z);
	void setPos(float x, float y, float z);
	//
	std::vector<float *> getAngles();
	void addAngle(float a, float x, float y, float z);
	void clearAngles();
	//
	void getScale(float *x, float *y, float *z);
	void setScale(float x, float y, float z);
	//
	void setObjData(objLoader *data);
	objLoader *getObjData();
	//
	void setTextures(GLuint *textures);
	GLuint *getTextures();
	//
	virtual void draw();
	//
	void printModelInfo();
private:
	float pos_x, pos_y, pos_z;
	std::vector<float *> rot_list;
	float scale_x, scale_y, scale_z;
	//
	char model_name[OBJ_FILENAME_LENGTH];
	objLoader *objData;
	GLuint *textures;
	//
	GLuint loadImage(const char* lpszPathName, int flag);
	void printVector(obj_vector *v);
	obj_vector *calcNormal(obj_face *o);
	void normalize(obj_vector *v);
	//
	BoudingBox *bbox;
};

#endif
