#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "globals.h"
#include "object.h"

class Camera{
public:
	Camera();
	Camera(VertexF position);
	~Camera();
	void LookAt(VertexF tgt);
	void LookAt(Object *tgt);
	void MoveTo(VertexF tgt);
	void LookThrough(); // I know, it's corny ;D
private:
	VertexF pos;
};

#endif
