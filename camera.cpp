#include "camera.h"
#include "globals.h"
#include "object.h"

Camera::Camera(){
	Vertex3 p = {0,0,0};
	pos = p;
}

Camera::Camera(Vertex3 position){
	pos = position;
}

Camera::~Camera(){
}

void Camera::LookAt(Vertex3 tgt){
}

void Camera::LookAt(Object *tgt){
}
