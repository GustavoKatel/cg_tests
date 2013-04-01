#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	static Camera *getCamera();
	//
	void setPos(float px, float py, float pz);
	void getPos(float *px, float *py, float *pz);
	//
	void setLook(float lx, float ly, float lz);
	void getLook(float *lx, float *ly, float *lz);
	//
	void setUp(float ux, float uy, float uz);
	void getUp(float *ux, float *uy, float *uz);
	//
	void setViewport(float width, float height);
	void getViewport(float *w, float *y);
	//
	void translate(float dx, float dy, float dz);
	//
	void translate_look(float ldx, float ldy);
	//
	float inc(float v1, float v2, float x, float y, float z);
	//
	void update();
protected:
	Camera();
	virtual ~Camera();
	//
	static Camera *camera;
	
private:
	float pos_x, pos_y, pos_z;
	float look_x, look_y, look_z;
	float up_x, up_y, up_z;
	float viewport_w, viewport_h;
	//
	

};


#endif
