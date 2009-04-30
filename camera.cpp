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
	VertexF p = {0,0,0};
	pos = p;
}

Camera::Camera(VertexF position){
	pos = position;
}

Camera::~Camera(){
}

void Camera::LookAt(VertexF tgt){
}

void Camera::LookAt(Object *tgt){
}

void Camera::MoveTo(VertexF tgt){
	pos = tgt;
}

void Camera::LookThrough(){
	glLoadIdentity();
	glTranslatef(-pos.x,-pos.y,-pos.z);
}
