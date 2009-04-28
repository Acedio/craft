#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "globals.h"
#include "object.h"

class Camera{
public:
	Camera();
	Camera(Vertex3 position);
	~Camera();
	void LookAt(Vertex3 tgt);
	void LookAt(Object *tgt);
private:
	Vertex3 pos;
};

#endif
