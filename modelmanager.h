#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <vector>
#include <string>
#include <map>
using namespace std;

#include "globals.h"
#include "texturemanager.h"

struct Triangle{
	int v[3];
	int vt[3];
	int vn[3];
};

class ModelPiece{
public:
	ModelPiece(string n);
	void Draw();
private:
	string name;
	vector<VertexF> vertices;
	vector<PointF> tex_coords;
	vector<VertexF> normals;
	vector<Triangle> triangles;
	bool textured;
	TextureRef texture;
	friend class Model;
};

typedef unsigned int ModelRef;

class Model{
public:
	~Model();
	void Draw();
	int Load(string filename);
private:
	vector<ModelPiece*> pieces;
};

class ModelManager{
public:
	ModelManager();
	~ModelManager();
	ModelRef LoadModel(string filename);
private:
	map<ModelRef,Model*> models;
	map<string,ModelRef> filenames;
	ModelRef next_unused_ref;
};

#endif
