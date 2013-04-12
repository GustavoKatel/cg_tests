#include "bbox.h"

#include <iostream>

BoudingBox::BoudingBox(objLoader *objData)
{
	bx1 = by1 = bz1 = 0.0;
	bx2 = by2 = bz2 = 0.0;
	//
	sx = sy = sz = 1.0;
	//
	for(int i=0; i<objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];

		int t = 0;
		for(t=0;t<3;t++)
		{
			//0
			if(bx1>objData->vertexList[o->vertex_index[t]]->e[0])
				bx1 = objData->vertexList[o->vertex_index[t]]->e[0];
			if(bx2<objData->vertexList[o->vertex_index[t]]->e[0])
				bx2 = objData->vertexList[o->vertex_index[t]]->e[0];
			//1
			if(by1>objData->vertexList[o->vertex_index[t]]->e[1])
				by1 = objData->vertexList[o->vertex_index[t]]->e[1];
			if(by2<objData->vertexList[o->vertex_index[t]]->e[1])
				by2 = objData->vertexList[o->vertex_index[t]]->e[1];
			//2
			if(bz1>objData->vertexList[o->vertex_index[t]]->e[2])
				bz1 = objData->vertexList[o->vertex_index[t]]->e[2];
			if(bz2<objData->vertexList[o->vertex_index[t]]->e[2])
				bz2 = objData->vertexList[o->vertex_index[t]]->e[2];
		}
	}

	std::cout<<"bx1: "<<bx1<<std::endl;

}


BoudingBox::~BoudingBox()
{

}

int BoudingBox::is_touched(float x, float y, float z)
{
	//bx1*sx ...
	return 0;
}

void BoudingBox::setScale(float x, float y, float z)
{
	sx = x;
	sy = y;
	sz = z;
}

void BoudingBox::draw()
{
	//draw bounding box
	glColor3f(1,0,0);
	glBegin(GL_TRIANGLES);
	//front face
	glVertex3f(bx1, by1, bz1);
	glVertex3f(bx2, by1, bz1);
	glVertex3f(bx2, by2, bz1);

	glVertex3f(bx1, by1, bz1);
	glVertex3f(bx1, by2, bz1);
	glVertex3f(bx2, by2, bz1);
	//right face
	glColor3f(0,1,0);
	glVertex3f(bx2, by1, bz1);
	glVertex3f(bx2, by1, bz2);
	glVertex3f(bx2, by2, bz2);

	glVertex3f(bx2, by1, bz1);
	glVertex3f(bx2, by2, bz1);
	glVertex3f(bx2, by2, bz2);
	//left face
	glVertex3f(bx1, by1, bz1);
	glVertex3f(bx1, by1, bz2);
	glVertex3f(bx1, by2, bz2);

	glVertex3f(bx1, by1, bz1);
	glVertex3f(bx1, by2, bz1);
	glVertex3f(bx1, by2, bz2);
	//top face
	glVertex3f(bx1, by2, bz1);
	glVertex3f(bx2, by2, bz1);
	glVertex3f(bx2, by2, bz2);

	glVertex3f(bx1, by2, bz1);
	glVertex3f(bx1, by2, bz2);
	glVertex3f(bx2, by2, bz2);
	//bottom face
	
	glVertex3f(bx1, by1, bz1);
	glVertex3f(bx2, by1, bz1);
	glVertex3f(bx2, by1, bz2);

	glVertex3f(bx1, by1, bz1);
	glVertex3f(bx1, by1, bz2);
	glVertex3f(bx2, by1, bz2);
	//back face
	glColor3f(1,0,0);
	glVertex3f(bx1, by1, bz2);
	glVertex3f(bx2, by1, bz2);
	glVertex3f(bx2, by2, bz2);

	glVertex3f(bx1, by1, bz2);
	glVertex3f(bx1, by2, bz2);
	glVertex3f(bx2, by2, bz2);

	
	glEnd();
}
