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

struct ModelPiece{
	string name;
	vector<Triangle> triangles;
	bool textured;
	TextureRef texture;
};

typedef unsigned int ModelRef;

struct Model{
	vector<VertexF> vertices;
	vector<PointF> tex_coords;
	vector<VertexF> normals;
	vector<ModelPiece*> pieces;
};

class ModelManager{
public:
	ModelManager();
	~ModelManager();
	ModelRef LoadModel(string filename, TextureManager* textureManager);
	void UnloadModel(ModelRef ref);
	void DrawModel(ModelRef ref, TextureManager* textureManager);
private:
	map<ModelRef,Model*> models;
	map<string,ModelRef> filenames;
	ModelRef next_unused_ref;
};

#endif
