#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "globals.h"

class Camera{
public:
	Camera();
	Camera(VertexF position);
	Camera(VertexF position, PointF direction);
	~Camera();
	void MoveTo(VertexF position);
	void ChangeAngle(PointF a);
	void LookThrough(); // I know, it's corny ;D
	VertexF GetCamPos();
private:
	VertexF pos;
	PointF angle;
};

#endif
