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
	float theta;
	VertexF n;
	JointState* next;
};

struct Animation{
	vector<JointState*> keyFrames;
	vector<JointState*> frameDeltas; // these are the rotational velocities that are added to keyframe thetas (keyLength times) to move onto the next state
	vector<int> keyLengths;
};

class AnimationInstance{
public:
	AnimationInstance();
	AnimationInstance(Animation* animation);
	~AnimationInstance();
	void AdvanceFrames(int frames);
private:
	unsigned int key;
	int frame;
	JointState* currentDelta;
	Animation* animation;
	friend class ModelManager;
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
	bool teamColored; // are we coloring this piece the teams color?
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
	void DrawModel(ModelRef ref, TextureManager* textureManager, float cr = 1, float cg = 1, float cb = 1, AnimationInstance *animationInstance = NULL);
	AnimationInstance GetAnimationInstance(ModelRef modelRef, string animationName);
private:
	vector<ModelPiece*> LoadObj(string filename, Model* model, TextureManager* textureManager);
	void DrawPiece(Model* model, ModelPiece* piece, TextureManager* textureManager, float cr, float cg, float cb, JointState** initials, JointState** vels);
	Animation* MakeAnimation(vector<vector<VertexF> > frames, vector<int> frameLengths);
	map<ModelRef,Model*> models;
	map<string,ModelRef> filenames;
	ModelRef next_unused_ref;
	TextureRef lastTexture;
};

#endif
