// Sandy Hill, November 19, 1999 for ECE660 
// a Camera class – partial implementation - for flying through 3D scenes. 
#include "stdafx.h"

#include "camera.h" 


Vector3 Vector3::cross(Vector3 b) //return this cross b
{
	Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	return c;
}
float Vector3::dot(Vector3 b) // return this dotted with b
{
	return x * b.x + y * b.y + z * b.z;
}

void Vector3::normalize()//adjust this vector to unit length
{
	double sizeSq = x * x + y * y + z * z;
	if (sizeSq < 0.0000001)
	{
		return; // does nothing to zero vectors;
	}
	float scaleFactor = 1.0 / (float)sqrt(sizeSq);
	x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
}


Camera::Camera(void)
{ // make default camera 
	setDefaultCamera();
}
//<<<<<<<<<<<<<<<<<<<<<<< default constructor >>>>>>>>>>>>>>>>> 
void Camera::setDefaultCamera()
{ // make default camera 



	setShape(45.0f, 1366 / (float)705, 1.0f, 600.0f); // good default values here 

	Point3 eyePoint = Point3(15.0, 8.0, 15.0);
	Point3 lookPoint = Point3(0.0, 0.0, 0.0);

	Vector3 upVector = Vector3(0, 1, 0);

	set(eyePoint, lookPoint, upVector);
	//set(5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0, 1, 0); 
}
void Camera::panelSlide(double du, double dv, double dn)
{
	dv = 0;
	eye.x += du * u.x + dv * v.x + dn * n.x;
	eye.z += du * u.z + dv * v.z + dn * n.z;
	look.x += du*u.x + dv*v.x + dn*n.x;
	look.z += du*u.z + dv*v.z + dn*n.z;
	setModelViewMatrix();
}
void Camera::turnLeftAndRight(float angle)
{
	float ang = 3.14159265 / 180 * angle;
	float c = cos(ang), s = sin(ang);
	
	Vector3 diff(look.x - eye.x, 0, look.z - eye.z);

	Vector3 newPosdiff(c*diff.x - s*diff.z,
		0,
		s*diff.x + c*diff.z);

	look.x = eye.x + newPosdiff.x;
	look.z = eye.z + newPosdiff.z;

	Vector3 up(0.0, 1.0, 0.0);

	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	u.set(up.cross(n));
	v.set(n.cross(u));
	u.normalize(); v.normalize(); n.normalize();
	setModelViewMatrix();

}
void Camera::turnUpAndDown(float angle)
{
	angle = -angle;

	//防止视野过高过低
	/*if (angle >= 0 && look.y > 100) return;
	if (angle < 0 && look.y < -130) return;*/

	float ang = 3.14159265 / 180 * angle;
	float c = cos(ang), s = sin(ang);

	GLfloat ceX = look.x - eye.x;
	GLfloat ceZ = look.z - eye.z;
	GLfloat distanceCE = sqrt(ceX*ceX + ceZ*ceZ);
	Vector3 diff(distanceCE, look.y - eye.y, 0);
	Vector3 newPosdiff(diff.x*c - diff.y*s,
		diff.x*s + diff.y*c,
		0);
	look.y = eye.y + newPosdiff.y;
	GLfloat percent = newPosdiff.x / distanceCE;
	look.x = eye.x + percent*ceX;
	look.z = eye.z + percent*ceZ;

	Vector3 up(0.0, 1.0, 0.0);

	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	u.set(up.cross(n));
	v.set(n.cross(u));
	u.normalize(); v.normalize(); n.normalize();
	setModelViewMatrix();

}
//<<<<<<<<<<<<<<<<<<<< set >>>>>>>>>>>>>>>>>>> 
void Camera::set(float eyeX, float eyeY, float eyeZ, float lookX, float lookY, float lookZ, float upX, float upY, float upZ)
{


	Point3 Eye = Point3(eyeX, eyeY, eyeZ);
	Point3 look = Point3(lookX, lookY, lookZ);

	Vector3 up = Vector3(upX, upY, upZ);

	eye.set(Eye);
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	u.set(up.cross(n));
	v.set(n.cross(u));
	u.normalize(); v.normalize(); n.normalize();
	setModelViewMatrix();


}

//<<<<<<<<<<<<<<<<<<<< set >>>>>>>>>>>>>>>>>>> 
void Camera::set(Point3 Eye, Point3 look, Vector3 up)
{ // make u, v, n vectors 



	eye.set(Eye);
	n.set(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	u.set(up.cross(n));
	v.set(n.cross(u));
	u.normalize(); v.normalize(); n.normalize();
	setModelViewMatrix();
}
//<<<<<<<<<<<<<<<< setModelViewMatrix >>>>>>>>>>>>>>>> 
void Camera::setModelViewMatrix(void)
{ // load modelview matrix with existing camera values 


	float m[16];
	Vector3 eVec(eye.x, eye.y, eye.z);
	m[0] = u.x; m[4] = u.y; m[8] = u.z; m[12] = -eVec.dot(u);
	m[1] = v.x; m[5] = v.y; m[9] = v.z; m[13] = -eVec.dot(v);
	m[2] = n.x; m[6] = n.y; m[10] = n.z; m[14] = -eVec.dot(n);
	m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m);
}
//<<<<<<<<<<<<<<<<<<<<<<< setShape >>>>>>>>>>>>>>>> 
void Camera::setShape(float vAngle, float asp, float nr, float fr)
{ // load projection matrix and camera values 



	viewAngle = vAngle;
	aspect = asp;
	nearDist = nr;
	farDist = fr;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspect, nearDist, farDist);
	glMatrixMode(GL_MODELVIEW); // set its mode back again 
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<< setAspect >>>>>>>>>>>>>>>>>>>>>> 
void Camera::setAspect(float asp)
{
	
	aspect = asp;
}
//<<<<<<<<<<<<<<<<<<<<<<< getShape >>>>>>>>>>>>>>>> 
void Camera::getShape(float& vAngle, float& asp, float& nr, float& fr)
{


	vAngle = viewAngle;
	asp = aspect;
	nr = nearDist;
	fr = farDist;
}
//<<<<<<<<<<<<<<<<<<<<<< slide >>>>>>>>>>>>>>>>>>>>>>.. 
void Camera::slide(double du, double dv, double dn)
{ // slide both eye and lookAt by amount du * u + dv * v + dn * n; 

	eye.x += du * u.x + dv * v.x + dn * n.x;
	eye.y += du * u.y + dv * v.y + dn * n.y;
	eye.z += du * u.z + dv * v.z + dn * n.z;
	look.x += du*u.x + dv*v.x + dn*n.x;
	look.y += du*u.y + dv*v.y + dn*n.y;
	look.z += du*u.z + dv*v.z + dn*n.z;
	setModelViewMatrix();
}
//<<<<<<<<<<<<<<<<<<<<<<<< rotAxes >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 
void Camera::rotAxes(Vector3& a, Vector3& b, float angle)
{ // rotate orthogonal vectors a (like x axis) and b(like y axia) through angle degrees 
	float ang = 3.14159265 / 180 * angle;
	float C = cos(ang), S = sin(ang);
	Vector3 t(C * a.x + S * b.x, C * a.y + S * b.y, C * a.z + S * b.z); b.set(-S * a.x + C * b.x, -S * a.y + C * b.y,
		-S * a.z + C * b.z);
	a.set(t.x, t.y, t.z); // put tmp into a' 
}
//<<<<<<<<<<<<<<<<<<<<<<<< roll >>>>>>>>>>>>>>>>>>>>>>>>>>> 
void Camera::roll(float angle)
{

	rotAxes(u, v, angle);

	setModelViewMatrix();
}
//<<<<<<<<<<<<<<<<<<<<<<<< pitch >>>>>>>>>>>>>>>>>>>>>>>>> 
void Camera::pitch(float angle)
{
	rotAxes(n, v, angle);
	setModelViewMatrix();
}
//<<<<<<<<<<<<<<<<<<<<<<<<< yawCameraLeft >>>>>>>>>>>>>>>>>>>>>>>> 
void Camera::yaw(float angle)
{
	rotAxes(u, n, angle);
	setModelViewMatrix();
}

void Camera::resize(GLint w, GLint h)
{
	aspect = w*1.0 / h;
	glViewport(0, 0, w, h);
	setShape(viewAngle, aspect, nearDist, farDist);
}

void Camera::resetCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(viewAngle, aspect, nearDist, farDist);
	glMatrixMode(GL_MODELVIEW);

	Vector3 upVector = Vector3(0, 1, 0);
	set(eye, look, upVector);
}

Point3 Camera::getEyePosition()
{
	return eye;
}

void Camera::LookAtMatrix(Vector3 eye, Vector3 look, Vector3 up, float * matrix)
{
	Vector3 z(eye.x - look.x, eye.y - look.y, eye.z - look.z);
	z.normalize();
	Vector3 x(z.cross(up));
	x.normalize();
	Vector3 y(x.cross(z));
	y.normalize();

	matrix[0] = x.x;
	matrix[4] = x.y;
	matrix[8] = x.z;
	matrix[12] = -x.dot(eye);

	matrix[1] = y.x;
	matrix[5] = y.y;
	matrix[9] = y.z;
	matrix[13] = -y.dot(eye);

	matrix[2] = z.x;
	matrix[6] = z.y;
	matrix[10] = z.z;
	matrix[14] = -z.dot(eye);

	matrix[3] = matrix[7] = matrix[11] = 0.0f;
	matrix[15] = 1.0f;
}

void Camera::frustumMatrix(float l, float r, float b, float t, float zNear, float zFar, float * matrix)
{
	memset(matrix, 0, sizeof(matrix));
	matrix[0] = 2 * zNear / (r - l);
	matrix[8] = (r + l) / (r - l);
	matrix[5] = 2 * zNear / (t - b);
	matrix[9] = (t + b) / (t - b);
	matrix[10] = -(zFar + zNear) / (zFar - zNear);
	matrix[14] = -(2 * zFar * zNear) / (zFar - zNear);
	matrix[11] = -1;
	matrix[15] = 0;
}
