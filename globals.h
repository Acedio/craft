#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <string>
using namespace std;

struct VertexF{
	float x,y,z;
	VertexF operator-(VertexF b){VertexF temp; temp.x = x - b.x; temp.y = y - b.y; temp.z = z - b.z; return temp;}
	VertexF operator+(VertexF b){VertexF temp; temp.x = x + b.x; temp.y = y + b.y; temp.z = z + b.z; return temp;}
	VertexF operator*(int b){VertexF temp; temp.x = x * b; temp.y = y * b; temp.z = z * b; return temp;}
};

struct VertexI{
	int x,y,z;
};

struct PointF{
	float x,y;
	PointF operator+(PointF b){PointF temp; temp.x = x + b.x; temp.y = y + b.y; return temp;}
};

struct PointI{
	int x,y;
	bool operator<(const PointI b) const {if(x == b.x){ return y < b.y; } else { return x < b.x;}};
	PointI operator+(PointI b){PointI temp; temp.x = x + b.x; temp.y = y + b.y; return temp;}
	PointI operator*(int b){PointI temp; temp.x = x * b; temp.y = y * b; return temp;}
	bool operator==(const PointI b) const {return (x == b.x) && (y == b.y);};
};

string TrimFileName(string filepath);

#endif
