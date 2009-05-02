#ifndef _GLOBALS_H_
#define _GLOBALS_H_

struct VertexF{
	float x,y,z;
};

struct VertexI{
	int x,y,z;
};

struct PointF{
	float x,y;
};

struct PointI{
	int x,y;
	bool operator<(const PointI b) const {if(x == b.x) return y < b.y; else return x < b.x;};
};

#endif
