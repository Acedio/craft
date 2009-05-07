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
	VertexF temp = {0,0,0};
	pos = temp;
	dir = temp;
}

Camera::Camera(VertexF position){
	VertexF temp = {0,0,0};
	pos = position;
	dir = temp;
}

Camera::Camera(VertexF position, VertexF direction){
	pos = position;
	dir = direction;
}

Camera::~Camera(){
}

void Camera::MoveTo(VertexF position){
	pos = position;
}

void Camera::LookTo(VertexF direction){
	dir = direction;
}

void Camera::LookThrough(){
	glLoadIdentity();
	glRotatef(-180*atan2(dir.y,dir.z)/3.14159,1,0,0);
	glTranslatef(-pos.x,-pos.y,-pos.z);
}
