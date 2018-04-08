// Camera.h for my Camera class, Nov. 19,1999, for ECE660 
#ifndef _HlCamera 
#define _HlCamera 
#include "stdafx.h"
//##################### class CAMERA ###################### 
class Point3 {
public:
	float x, y, z;
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }
	void set(Point3& p) { x = p.x; y = p.y; z = p.z; }
	Point3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
	Point3() { x = y = z = 0; }
	void build4tuple(float v[])
	{// load 4-tuple with this color: v[3] = 1 for homogeneous
		v[0] = x; v[1] = y; v[2] = z; v[3] = 1.0f;
	}
};
class Vector3 {
public:
	float x, y, z;
	void set(float dx, float dy, float dz) { x = dx; y = dy; z = dz; }
	void set(Vector3& v) { x = v.x; y = v.y; z = v.z; }
	void flip() { x = -x; y = -y; z = -z; } // reverse this vector
	void setDiff(Point3& a, Point3& b)//set to difference a - b
	{
		x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;
	}
	void normalize();//adjust this vector to unit length
	Vector3(float xx, float yy, float zz) { x = xx; y = yy; z = zz; }
	Vector3(Vector3 const & v) { x = v.x; y = v.y; z = v.z; }
	Vector3() { x = y = z = 0; } //default constructor
	Vector3 cross(Vector3 b); //return this cross b
	float dot(Vector3 b); // return this dotted with b
};
class Camera {
private:
	Point3 eye, look;
	Vector3 u, v, n;
	float aspect, nearDist, farDist, viewAngle;
	void setModelViewMatrix();
public:
	Camera(void);
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	void slide(double du, double dv, double dn);
	void set(Point3 Eye, Point3 look, Vector3 up);

	void set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ);

	void setShape(float viewAngle, float aspect, float Near, float Far);
	void setAspect(float asp);
	void getShape(float& viewAngle, float& aspect, float& Near, float& Far);
	void rotAxes(Vector3& a, Vector3& b, float angle);
	void setDefaultCamera();
	void panelSlide(double du, double dv, double dn);
	void turnLeftAndRight(float angle);
	void turnUpAndDown(float angle);
	void resize(GLint w, GLint h);
	void resetCamera();
	Point3 getEyePosition();
	static void LookAtMatrix(Vector3 eye, Vector3 look, Vector3 up, float *matrix);
	static void frustumMatrix(float l, float r, float b, float t, float zNear, float zFar, float *matrix);
};
#endif 