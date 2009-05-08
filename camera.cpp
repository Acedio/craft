#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "camera.h"
#include "globals.h"
#include "object.h"

Camera::Camera(){
	VertexF tp = {0,0,0};
	PointF ta = {0,0};
	pos = tp;
	angle = ta;
}

Camera::Camera(VertexF position){
	pos = position;
	PointF ta = {0,0};
	angle = ta;
}

Camera::Camera(VertexF position, PointF a){
	pos = position;
	angle = a;
}

Camera::~Camera(){
}

void Camera::MoveTo(VertexF position){
	pos = position;
}

void Camera::ChangeAngle(PointF a){
	angle = a;
}

void Camera::LookThrough(){
	glLoadIdentity();
	glRotatef(-180*angle.x/3.14159,1,0,0);
	glRotatef(-180*angle.y/3.14159,0,1,0);
	glTranslatef(-pos.x,-pos.y,-pos.z);
}
