#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "globals.h"
#include "object.h"

class Camera{
public:
	Camera();
	Camera(VertexF position);
	Camera(VertexF position, VertexF direction);
	~Camera();
	void MoveTo(VertexF position);
	void LookTo(VertexF direction);
	void LookThrough(); // I know, it's corny ;D
private:
	VertexF pos;
	VertexF dir;
};

#endif
