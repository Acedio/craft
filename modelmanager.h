#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <vector>
#include <string>
#include <map>
#include <list>
using namespace std;

#include "globals.h"
#include "texturemanager.h"

struct JointState{
	float theta, tx, ty, tz;
};

typedef list<JointState> Frame;

struct Animation{
	vector<Frame> keyFrames;
	vector<float> frameDeltas; // these are added to keyframe thetas (keyLength times) to move onto the next state
	vector<int> keyLengths;
};

class AnimationInstance{
public:
	void NextFrame();
private:
	Frame current;
	Animation* animation;
};

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
	vector<ModelPiece*> children;
	VertexF joint;
	GLuint displayList;
};

struct Model{
	vector<VertexF> vertices;
	vector<PointF> tex_coords;
	vector<VertexF> normals;
	vector<ModelPiece*> pieces;
	map<string,Animation*> animations;
};

typedef unsigned int ModelRef;

class ModelManager{
public:
	ModelManager();
	~ModelManager();
	ModelRef LoadModel(string filename, TextureManager* textureManager);
	void UnloadModel(ModelRef ref);
	void DrawModel(ModelRef ref, TextureManager* textureManager);
private:
	vector<ModelPiece*> LoadObj(string filename, Model* model, TextureManager* textureManager);
	void DrawPiece(Model* model, ModelPiece* piece, TextureManager* textureManager);
	map<ModelRef,Model*> models;
	map<string,ModelRef> filenames;
	ModelRef next_unused_ref;
	TextureRef lastTexture;
};

#endif
