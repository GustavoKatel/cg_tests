#include "object.h"

#include <GL/glut.h>
#include <math.h>

#include "SOIL.h"
#include "camera.h"

Object::Object(const char *model_file_name)
{
	strcpy(model_name, model_file_name);
	//
	if(strlen(model_name))
	{
		objData = new objLoader();
		objData->load(model_name);
		loadTextures();
		bbox = new BoudingBox(objData);
	}else{
		objData = NULL;
		textures= NULL;
	}
	scale_x = 1;
	scale_y = 1;
	scale_z = 1;
	//
	pos_x = 0;
	pos_y = 0;
	pos_z = 0;
}

Object::~Object()
{
	if(objData)
		delete objData;
	unsigned int i;
	for(i=0;i<rot_list.size();i++)
		free(rot_list.at(i));
	rot_list.clear();
	//
	if(textures)
		free(textures);
}

void Object::loadTextures()
{
	int total=0;
	textures = (GLuint *)malloc(sizeof(GLuint)*objData->materialCount);
	if(!textures)
	{
		std::cout<<"Could not create the texture index list!"<<std::endl;
		exit(1);
	}
	int i;
	//
	for(i=0;i<objData->materialCount;i++)
	{
		textures[i]=0;
		obj_material *mtl = objData->materialList[i];
		if(!strlen(mtl->texture_filename))
		{
			std::cout<<"NO TEXTURE IMAGE FILENAME SPECIFIED!"<<std::endl;
			continue;
		}
		//fix \n in the end of the string
		char last_char = mtl->texture_filename[strlen(mtl->texture_filename)-1];
		while(last_char=='\n' || last_char=='\t')
		{
			mtl->texture_filename[strlen(mtl->texture_filename)-1]='\0';
			last_char = mtl->texture_filename[strlen(mtl->texture_filename)-1];
		}
		//
		/*		int img_width=0, img_height=0, img_channels=0;
				unsigned char *ntexture = SOIL_load_image
				(
				mtl->texture_filename,
				&img_width,
				&img_height,
				&img_channels,
				SOIL_LOAD_AUTO
				);*/
		GLuint ntexture = this->loadImage(mtl->texture_filename,0);
		if(!ntexture)
		{
			std::cout<<"Error trying to load image: "<<mtl->texture_filename<<" texture ptr: "<<ntexture<<std::endl;
			continue;
		}
		total++;
		textures[i]=ntexture;
		//		glGenTextures(1,&textures[i]);
		std::cout<<"texture generated: "<<textures[i]<<std::endl;
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		//		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ntexture);
		//gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, img_width, img_height, GL_RGBA, GL_UNSIGNED_BYTE, ntexture);
		//std::cout<<"i: "<<i<<std::endl;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//
		//		SOIL_free_image_data(ntexture);	
	}
}

void Object::getPos(float *x, float *y, float *z)
{
	*x = pos_x;
	*y = pos_y;
	*z = pos_z;

}

void Object::setPos(float x, float y, float z)
{
	this->pos_x = x;
	this->pos_y = y;
	this->pos_z = z;
}

std::vector<float *> Object::getAngles()
{
	return rot_list;
}
void Object::addAngle(float a, float x, float y, float z)
{
	float *rot = (float *)malloc(sizeof(float)*4);
	rot[0] = a;
	rot[1] = x;
	rot[2] = y;
	rot[3] = z;
	rot_list.push_back(rot);
}
void Object::clearAngles()
{
	unsigned int i;
	for(i=0;i<rot_list.size();i++)
		free(rot_list.at(i));
	rot_list.clear();
}

void Object::getScale(float *x, float *y, float *z)
{
	*x = scale_x;
	*y = scale_y;
	*z = scale_z;
}

void Object::setScale(float x, float y, float z)
{
	this->scale_x = x;
	this->scale_y = y;
	this->scale_z = z;
	//
	if(bbox)
	{
		bbox->setScale(scale_x, scale_y, scale_z);
	}
}
//
void Object::setObjData(objLoader *data)
{
	this->objData = data;
}
objLoader *Object::getObjData()
{
	return this->objData;
}
//
void Object::setTextures(GLuint *textures)
{
	this->textures=textures;
}
GLuint *Object::getTextures()
{
	return this->textures;
}
//
void Object::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//
	glLoadIdentity();
	//Camera
	float cpx, cpy, cpz;
	Camera::getCamera()->getPos(&cpx, &cpy, &cpz);
	float angle_x, angle_y;
	Camera::getCamera()->getLookAngle(&angle_x, &angle_y);
	//
	//
	glTranslatef(-cpx, -cpy, -cpz);
	//camera rotate
	glRotatef(angle_x, 0,1,0);
	glRotatef(angle_y, 1,0,0);
	//
	glTranslatef(this->pos_x, 
			this->pos_y, 
			this->pos_z);
	//
	unsigned int i;
	for(i=0;i<rot_list.size();i++)
		glRotatef(rot_list.at(i)[0],
				rot_list.at(i)[1],
				rot_list.at(i)[2],
				rot_list.at(i)[3]);
	//
	glScalef( this->scale_x, this->scale_y, this->scale_z);
	//
	bbox->draw();
	//
	for(int i=0; i<objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];

		int freeNormal = 0;
		obj_vector *normal_0 = NULL, *normal_1 = NULL, *normal_2 = NULL;
		if(o->normal_index[0]>=0)
			normal_0 = objData->normalList[o->normal_index[0]];
		if(o->normal_index[1]>=0)
			normal_1 = objData->normalList[o->normal_index[1]];
		if(o->normal_index[2]>=0)
			normal_2 = objData->normalList[o->normal_index[2]];
		if(!normal_0 && !normal_1 && !normal_2)
		{
			normal_0 = normal_1 = normal_2 = calcNormal(o);
			freeNormal=1;
		}else
			freeNormal=0;

		obj_material *mtl = NULL;
		GLuint tex = 0;
		if(o->material_index>=0)
		{
			tex = textures[o->material_index];
			mtl = objData->materialList[o->material_index];
		}

		//
		GLfloat params[4];
		params[0] = 0.5;
		params[1] = 0.5;
		params[2] = 0.5;
		params[3] = 1.0;
		//ambient
		if(mtl)
		{
			params[0] = mtl->amb[0];
			params[1] = mtl->amb[1];
			params[2] = mtl->amb[2];
		}
		glMaterialfv(GL_FRONT, GL_AMBIENT, params);
		//diffuse
		if(mtl)
		{
			params[0] = mtl->diff[0];
			params[1] = mtl->diff[1];
			params[2] = mtl->diff[2];
		}
		glMaterialfv(GL_FRONT, GL_DIFFUSE, params);
		//specular
		if(mtl)
		{
			params[0] = mtl->spec[0];
			params[1] = mtl->spec[1];
			params[2] = mtl->spec[2];
		}
		glMaterialfv(GL_FRONT, GL_SPECULAR, params);
		//
		glMaterialf(GL_FRONT, GL_SHININESS, ( mtl ? mtl->shiny : 64 ) );
		//

		int has_tex = (tex>=0) && mtl;

		if(has_tex)
			glBindTexture(GL_TEXTURE_2D, tex);

		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 1.0, 1.0);
		//
		//------------- vertex 1 -------------
		//------------- normal ---------------
		if( normal_0 )
			glNormal3f(normal_0->e[0],
					normal_0->e[1],
					normal_0->e[2] );
		//------------- texture coord --------
		if(has_tex && o->texture_index[0]>=0 )
			glTexCoord2f(   objData->textureList[o->texture_index[0]]->e[0],
					objData->textureList[o->texture_index[0]]->e[1] );
		//------------- coord ----------------
		glVertex3f(	objData->vertexList[o->vertex_index[0]]->e[0], // primeiro vértice
				objData->vertexList[o->vertex_index[0]]->e[1],
				objData->vertexList[o->vertex_index[0]]->e[2]);

		//------------- vertex 2 -------------
		//------------- normal ---------------
		if( normal_1 )
			glNormal3f(normal_1->e[0],
					normal_1->e[1],
					normal_1->e[2] );
		//------------- texture coord --------
		if(has_tex && o->texture_index[1]>=0)
			glTexCoord2f(   objData->textureList[o->texture_index[1]]->e[0],
					objData->textureList[o->texture_index[1]]->e[1] );
		//------------- coord ----------------
		glVertex3f(	objData->vertexList[o->vertex_index[1]]->e[0], // segundo vértice
				objData->vertexList[o->vertex_index[1]]->e[1],
				objData->vertexList[o->vertex_index[1]]->e[2]);

		//------------- vertex 3 -------------
		//------------- normal ---------------
		if( normal_2 )
			glNormal3f(normal_2->e[0],
					normal_2->e[1],
					normal_2->e[2] );
		//------------- texture coord --------
		if(has_tex && o->texture_index[2]>=0)
			glTexCoord2f(   objData->textureList[o->texture_index[2]]->e[0],
					objData->textureList[o->texture_index[2]]->e[1] );
		//------------- coord ----------------
		glVertex3f(	objData->vertexList[o->vertex_index[2]]->e[0],	// terceiro vértice
				objData->vertexList[o->vertex_index[2]]->e[1],
				objData->vertexList[o->vertex_index[2]]->e[2]);

		glEnd();
		//
		if(freeNormal && normal_0)
		{
			free(normal_0);
			freeNormal=0;
		}
	}
	glPopMatrix();
}


GLuint Object::loadImage(const char* lpszPathName, int flag) {
	return SOIL_load_OGL_texture(
			lpszPathName,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT   
			//SOIL_FLAG_INVERT_Y
			);
}

// Funcao que imprime as coordenadas de um vertice.
// Pode ser utilizada para fazer debug de código.
void Object::printVector(obj_vector *v)
{
	std::cout<< v->e[0] <<std::endl;
	std::cout<< v->e[1] <<std::endl;
	std::cout<< v->e[2] <<std::endl;
}
// Esta funcao apenas imprime informacoes referentes ao modelo que foi carregado,
// tais como numero de vertices, normais, fontes de luz, etc.
void Object::printModelInfo()
{
	std::cout<<"Number of vertices: "<< objData->vertexCount<<std::endl;
	std::cout<<"Number of vertex normals: "<< objData->normalCount<<std::endl;
	std::cout<<"Number of texture coordinates: "<< objData->textureCount<<std::endl;

	std::cout<<"Number of faces: "<< objData->faceCount<<std::endl;

	for(int i=0; i<objData->faceCount; i++)
	{
		obj_face *o = objData->faceList[i];
		std::cout<<" face ";
		for(int j=0; j<3; j++)
		{
			printVector(objData->vertexList[ o->vertex_index[j] ]);
		}
		std::cout<<std::endl;
	}

	std::cout<<std::endl;

	std::cout<<"Number of spheres: "<< objData->sphereCount<<std::endl;;
	for(int i=0; i<objData->sphereCount; i++)
	{
		obj_sphere *o = objData->sphereList[i];
		std::cout<<" sphere ";
		printVector(objData->vertexList[ o->pos_index ]);
		printVector(objData->normalList[ o->up_normal_index ]);
		printVector(objData->normalList[ o->equator_normal_index ]);
		std::cout<<"\n";

	}

	std::cout<<"\n";

	std::cout<<"Number of planes: "<< objData->planeCount<<std::endl;
	for(int i=0; i<objData->planeCount; i++)
	{
		obj_plane *o = objData->planeList[i];
		std::cout<<" plane "<<std::endl;
		printVector(objData->vertexList[ o->pos_index ]);
		printVector(objData->normalList[ o->normal_index]);
		printVector(objData->normalList[ o->rotation_normal_index]);
		std::cout<<"\n";
	}

	std::cout<<"\n";

	std::cout<<"Number of point lights: "<< objData->lightPointCount<<std::endl;
	for(int i=0; i<objData->lightPointCount; i++)
	{
		obj_light_point *o = objData->lightPointList[i];
		std::cout<<" plight ";
		printVector(objData->vertexList[ o->pos_index ]);
		std::cout<<"\n";
	}

	std::cout<<"\n";

	std::cout<<"Number of disc lights: "<< objData->lightDiscCount<<std::endl;;
	for(int i=0; i<objData->lightDiscCount; i++)
	{
		obj_light_disc *o = objData->lightDiscList[i];
		std::cout<<" dlight ";
		printVector(objData->vertexList[ o->pos_index ]);
		printVector(objData->normalList[ o->normal_index ]);
		std::cout<<"\n";
	}

	std::cout<<"\n";

	std::cout<<"Number of quad lights: "<< objData->lightQuadCount<<std::endl;
	for(int i=0; i<objData->lightQuadCount; i++)
	{
		obj_light_quad *o = objData->lightQuadList[i];
		std::cout<<" qlight ";
		printVector(objData->vertexList[ o->vertex_index[0] ]);
		printVector(objData->vertexList[ o->vertex_index[1] ]);
		printVector(objData->vertexList[ o->vertex_index[2] ]);
		printVector(objData->vertexList[ o->vertex_index[3] ]);
		std::cout<<"\n";
	}

	std::cout<<"\n";

	if(objData->camera != NULL)
	{
		std::cout<<"Found a camera\n";
		std::cout<<" position: ";
		printVector(objData->vertexList[ objData->camera->camera_pos_index ]);
		std::cout<<"\n looking at: ";
		printVector(objData->vertexList[ objData->camera->camera_look_point_index ]);
		std::cout<<"\n up normal: ";
		printVector(objData->normalList[ objData->camera->camera_up_norm_index ]);
		std::cout<<"\n";
	}

	std::cout<<"\n";

	std::cout<<"Number of materials: "<< objData->materialCount<<std::endl;
	for(int i=0; i<objData->materialCount; i++)
	{
		obj_material *mtl = objData->materialList[i];
		std::cout<<" name: "<< mtl->name;
		std::cout<<" amb: "<< mtl->amb[0]<<" ";
		std::cout<< mtl->amb[1]<<" ";
		std::cout<< mtl->amb[2]<<std::endl;

		std::cout<<" diff: "<< mtl->diff[0]<<" ";
		std::cout<< mtl->diff[1]<<" ";
		std::cout<< mtl->diff[2]<<std::endl;

		std::cout<<" spec: "<< mtl->spec[0]<<" ";
		std::cout<< mtl->spec[1]<<" ";
		std::cout<< mtl->spec[2]<<std::endl;

		std::cout<<" reflect: "<< mtl->reflect<<std::endl;
		std::cout<<" trans: "<< mtl->trans<<std::endl;
		std::cout<<" glossy: "<< mtl->glossy<<std::endl;
		std::cout<<" shiny: "<< mtl->shiny<<std::endl;
		std::cout<<" refact: "<< mtl->refract_index<<std::endl;

		std::cout<<" texture: "<< mtl->texture_filename<<std::endl;
		std::cout<<"\n";
	}

	std::cout<<"\n";

	//vertex, normal, and texture test
	if(objData->textureCount > 2 && objData->normalCount > 2 && objData->faceCount > 2)
	{
		std::cout<<"Detailed face data:\n";

		for(int i=0; i<3; i++)
		{
			obj_face *o = objData->faceList[i];
			std::cout<<" face ";
			for(int j=0; j<3; j++)
			{
				std::cout<< o->vertex_index[j] <<"/";
				std::cout<< o->texture_index[j] <<"/";
				std::cout<< o->normal_index[j] ;
			}
			std::cout<<"\n";
		}
	}
}

obj_vector *Object::calcNormal(obj_face *o)
{

	obj_vector *v1 = objData->vertexList[o->vertex_index[0]];
	obj_vector *v2 = objData->vertexList[o->vertex_index[1]];
	obj_vector *v3 = objData->vertexList[o->vertex_index[2]];

	obj_vector u, v;
	u.e[0] = v2->e[0] - v1->e[0];
	u.e[1] = v2->e[1] - v1->e[1];
	u.e[2] = v2->e[2] - v1->e[2];
	//
	v.e[0] = v3->e[0] - v1->e[0];
	v.e[1] = v3->e[1] - v1->e[1];
	v.e[2] = v3->e[2] - v1->e[2];

	obj_vector *vn = (obj_vector*)malloc(sizeof(*vn));

	// vn = v1 x v2;
	vn->e[0] = u.e[1]*v.e[2] - u.e[2]*v.e[1];
	vn->e[1] = u.e[2]*v.e[0] - u.e[0]*v.e[2];
	vn->e[2] = u.e[0]*v.e[1] - u.e[1]*v.e[0];

	normalize(vn);

	return vn;
}

void Object::normalize(obj_vector *v)
{
	float s = sqrt( pow(v->e[0],2) +
			pow(v->e[1],2) +
			pow(v->e[2],2)  );
	if(!s)
		return;
	v->e[0] = v->e[0]/s;
	v->e[1] = v->e[1]/s;
	v->e[2] = v->e[2]/s;
}
