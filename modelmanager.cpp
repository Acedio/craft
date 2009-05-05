#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <sstream>
#include <string>
#include <cmath>
#include <iostream>
#include <cassert>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>

#include "globals.h"
#include "modelmanager.h"
#include "texturemanager.h"

AnimationInstance::AnimationInstance(Animation* a){
	animation = a;
	key = 0;
	frame = 0;
	currentDelta = NULL;
	if(a != NULL){
		JointState *delta = animation->frameDeltas[frame];
		JointState *deltaCopy = new JointState;
		currentDelta = deltaCopy;
		if(delta != NULL){
			*deltaCopy = *delta;
			while(delta->next != NULL){
				deltaCopy->next = new JointState;
				deltaCopy = deltaCopy->next;
				delta = delta->next;
				*deltaCopy = *delta;
			}
			deltaCopy->next = NULL;
		}
	}
}

AnimationInstance::~AnimationInstance(){
	while(currentDelta != NULL){
		JointState *temp = currentDelta->next;
		delete currentDelta;
		currentDelta = temp;
	}
}

void AnimationInstance::NextFrame(){
	if(animation != NULL){
		frame++;
		if(frame > animation->keyLengths[key]){
			frame = 0;
			key++;
			if(key >= animation->keyFrames.size()){
				key = 0;
			}
			// load the new frame deltas
			JointState *delta = animation->frameDeltas[key];
			JointState *deltaCopy = currentDelta;
			while(delta != NULL){
				deltaCopy->theta = 0;
				deltaCopy->n.x = delta->n.x;
				deltaCopy->n.y = delta->n.y;
				deltaCopy->n.z = delta->n.z;
				deltaCopy = deltaCopy->next;
				delta = delta->next;
			}
		} else { // otherwise just update the rotation angles
			JointState *delta = animation->frameDeltas[key];
			JointState *deltaCopy = currentDelta;
			while(delta != NULL && deltaCopy != NULL){
				deltaCopy->theta += delta->theta;
				deltaCopy = deltaCopy->next;
				delta = delta->next;
			}
		}
	}
}

ModelManager::ModelManager(){
	next_unused_ref = 0;
}

ModelManager::~ModelManager(){
	for(map<ModelRef,Model*>::iterator m = models.begin(); m != models.end(); ++m){
		if(m->second != NULL){
			for(vector<ModelPiece*>::iterator piece = m->second->pieces.begin(); piece != m->second->pieces.end(); ++piece){
				queue<ModelPiece*> pieceQueue;
				pieceQueue.push(*piece);
				while(!pieceQueue.empty()){
					if(pieceQueue.front() != NULL){
						for(vector<ModelPiece*>::iterator p = (pieceQueue.front())->children.begin(); p != (pieceQueue.front())->children.end(); ++p){
							pieceQueue.push(*p);
						}
						delete pieceQueue.front();
					} else {
						cout << "NULL piece in model." << endl;
					}
					pieceQueue.pop();
				}
			}
			delete m->second;
		}
	}
}

vector<ModelPiece*> ModelManager::LoadObj(string filename, Model* model, TextureManager* textureManager){
	vector<ModelPiece*> pieces;

	ifstream fin;
	fin.open(filename.c_str());
	if(fin.fail()){
		cout << "Could not open obj file \"" + filename + "\"." << endl;
		return pieces;
	}
	
	string s;

	bool failure = false;

	ModelPiece* piece = NULL;

	while(!getline(fin,s).eof()){
		stringstream line(s,stringstream::in | stringstream::out);
		string arg;
		getline(line,arg,' ');
		if(arg == "#"){
		} else if(arg == "o"){
			if(piece != NULL){
				pieces.push_back(piece);
			}
			piece = new ModelPiece;
			getline(line,piece->name,' ');
			piece->textured = false;
			VertexF joint;
			joint.x = joint.y = joint.z = 0;
			piece->joint = joint;
			piece->displayList = 0;
			cout << "Loading ModelPiece \"" << piece->name << "\"." << endl;
		} else if(piece != NULL){ // if we've named a piece we can go on and add stuff to it
			if(arg == "v"){
				VertexF v;
				line >> v.x;
				line >> v.y;
				line >> v.z;
				model->vertices.push_back(v);
			} else if(arg == "vt"){
				piece->textured = true;
				PointF vt;
				line >> vt.x;
				line >> vt.y;
				model->tex_coords.push_back(vt);
			} else if(arg == "vn"){
				VertexF vn;
				line >> vn.x;
				line >> vn.y;
				line >> vn.z;
				model->normals.push_back(vn);
			} else if(arg == "f"){
				Triangle t;
				for(int i = 0; i < 3; ++i){ // 3 times for each face (triangle)
					string temp;
					stringstream s;

					getline(line,temp,'/');
					s << temp << " ";
					temp.clear();
					getline(line,temp,'/');
					if(piece->textured){
						s << temp << " ";
					} else {
						s << "1 ";
					}
					temp.clear();
					getline(line,temp,' ');
					s << temp << " ";
					temp.clear();
					s >> t.v[i] >> t.vt[i] >> t.vn[i];
					--t.v[i];
					--t.vt[i];
					--t.vn[i];
				}
				piece->triangles.push_back(t);
			} else if(arg == "usemtl"){
				string texfile;
				getline(line,texfile);
				piece->texture = textureManager->LoadTexture(texfile);
			}
		} else { // we aren't creating a new piece and no piece have been created
			cout << "Piece accessed before first object named!" << endl;
			failure = true; 
			break;
		}
	}
	if(piece != NULL){
		pieces.push_back(piece); // push back the last piece (as long as we have a piece being defined, that is)
	}

	fin.close();

	if(failure){
		cout << "Couldn't load obj file \"" << filename << "\" successfully." << endl;
		while(!pieces.empty()){
			if(pieces.back() != NULL){
				delete pieces.back();
			}
			pieces.pop_back();
		}
	}
	return pieces;
}

Animation* ModelManager::MakeAnimation(vector<vector<VertexF> > frames, vector<int> frameLengths){
	Animation* animation = new Animation;
	for(unsigned int frame = 0; frame < frames.size(); frame++){
		if(frames[frame].size() == frames[(frame+1)%frames.size()].size()){
			JointState *initials = new JointState, *vels = new JointState; // these are fake heads, we'll need to delete these.
			JointState *initial = initials, *vel = vels;
			for(unsigned int joint = 0; joint < frames[frame].size(); joint++){
				VertexF start = frames[frame][joint];
				VertexF end = frames[(frame+1)%frames.size()][joint];
				// the normal for all parts is originally 0 -1 0 (straight down), so we need to translate to this frame's initial position
				initial->next = new JointState;
				initial = initial->next;
				initial->n.x = -1*start.z-0;
				initial->n.y = 0;
				initial->n.z = 0-start.x*(-1);
				initial->theta = 180.0*acos(-1*start.y)/3.14159;
				// calculate the rotation normal between start and end
				vel->next = new JointState;
				vel = vel->next;
				vel->n.x = start.y*end.z-end.y*start.z;
				vel->n.y = start.z*end.x-end.z*start.x;
				vel->n.z = start.x*end.y-end.x*start.y;
				vel->theta = 180.0*acos(start.x*end.x+start.y*end.y+start.z*end.z)/frameLengths[frame]/3.14159;
				//cout << vel->theta << " " << vel->n.x << " " << vel->n.y << " " << vel->n.z << "\t\t";
			}
			//cout << endl;
			//cap off the lists
			initial->next = NULL;
			vel->next = NULL;
			//remove the false heads
			JointState *temp;
			temp = initials;
			initials = initials->next;
			delete temp;
			temp = vels;
			vels = vels->next;
			delete temp;

			animation->keyFrames.push_back(initials);
			animation->frameDeltas.push_back(vels);
			animation->keyLengths = vector<int>(frameLengths);
		} else {
			return NULL;
		}
	}
	return animation;
}

AnimationInstance ModelManager::GetAnimationInstance(ModelRef modelRef, string animationName){
	map<ModelRef,Model*>::iterator model = models.find(modelRef);
	if(model != models.end()){
		map<string,Animation*>::iterator animation = (model->second)->animations.find(animationName);
		if(animation != model->second->animations.end()){
			return AnimationInstance(animation->second);
		} else {
			cout << "Can't find animation named \"" << animationName << "\" for model." << endl;
			return AnimationInstance(NULL);
		}
	} else {
		cout << "Can't find model #" << modelRef << "." << endl;
		return AnimationInstance(NULL);
	}
}

ModelRef ModelManager::LoadModel(string filename, TextureManager* textureManager){
	map<string,ModelRef>::iterator f;
	if((f = filenames.find(filename)) != filenames.end()){
		return f->second;
	} else {
		Model* model = new Model();

		ifstream fin;
		fin.open(filename.c_str());
		if(fin.fail()){
			cout << "Could not open model file \"" + filename + "\"." << endl;
			return -1;
		}

		string s;

		bool failure = false;

		vector<ModelPiece*> objPieces; // TODO make me a list!

		string currentAnimation;

		vector<vector<VertexF> > curFrames;
		
		vector<int> curFrameLengths;

		while(!getline(fin,s).eof()){
			stringstream line(s,stringstream::in | stringstream::out);
			string arg;
			getline(line,arg,' ');
			if(arg == "#"){
			} else if(arg == "OBJ"){
				string objFile;
				getline(line,objFile);
				objPieces = LoadObj(objFile, model, textureManager);
			} else if(arg == "JOINT"){
				if(objPieces.empty()){
					cout << filename << ": JOINT found but no pieces exist. Are you missing an OBJ statement?" << endl;
				}
				string pieceName;
				getline(line,pieceName,' ');
				VertexF joint;
				line >> joint.x >> joint.y >> joint.z;
				vector<ModelPiece*>::iterator i;
				for(i = objPieces.begin(); i != objPieces.end(); ++i){
					if((*i)->name == pieceName){
						(*i)->joint = joint;
						break;
					}
				}
				if(i == objPieces.end()){
					cout << filename << ": Could not find model piece \"" << pieceName << "\". Joint not set." << endl;
				}
			} else if(arg == "PIECETREE"){
				if(objPieces.empty()){
					cout << filename << ": PIECETREE found but no pieces exist. Are you missing an OBJ statement?" << endl;
				}
				stack<ModelPiece*> pieceStack;
				while(!line.eof()){
					string s;
					line >> s;
					if(s == "["){
						if(model->pieces.empty()){ // no pieces defined yet, nothing to push!
							cout << filename << ": No pieces defined, cannot push onto piecetree!" << endl;
						} else {
							if(pieceStack.empty()){
								pieceStack.push(model->pieces.back());
							} else {
								pieceStack.push(pieceStack.top()->children.back());
							}
						}
					} else if(s == "]"){
						if(pieceStack.empty()){
							cout << filename << ": Unexpected ']': piecestack empty." << endl;
						} else {
							pieceStack.pop();
						}
					} else {
						vector<ModelPiece*>::iterator i;
						for(i = objPieces.begin(); i != objPieces.end(); ++i){
							if((*i)->name == s){
								break;
							}
						}
						if(i == objPieces.end()){
							cout << filename << ": Could not find model piece \"" << s << "\". Piecetree may be broken." << endl;
						} else {
							if(pieceStack.empty()){ // if we're not between [ ], the piece belongs to the "root"
								model->pieces.push_back(*i);
							} else { // otherwise it is a child piece
								pieceStack.top()->children.push_back(*i);
							}
							objPieces.erase(i); // no recursive pieces!
						}
					}
				}
				if(!pieceStack.empty()){
					cout << filename << ": Error: piecestack not empty. Missing ']'?" << endl;
				}
			} else if(arg == "ANIM"){
				if(line.eof()){
					cout << filename << ": Error: No animation name specified in ANIM statement!" << endl;
				} else {
					if(currentAnimation != ""){
						// TODO do calculations for frame interpolation here (and once afterwards.)
						model->animations[currentAnimation] = MakeAnimation(curFrames, curFrameLengths);
						if(model->animations[currentAnimation] == NULL){
							cout << filename << ": Error making animation \"" << currentAnimation << "\" from given keyframes." << endl;
							model->animations.erase(currentAnimation);
						}
						curFrames.clear();
						curFrameLengths.clear();
					}
					line >> currentAnimation;
				}
			} else if(arg == "ENDANIM"){
				if(currentAnimation != ""){
					model->animations[currentAnimation] = MakeAnimation(curFrames, curFrameLengths);
					if(model->animations[currentAnimation] == NULL){
						cout << filename << ": Error making animation \"" << currentAnimation << "\" from given keyframes." << endl;
						model->animations.erase(currentAnimation);
					}
					curFrames.clear();
					curFrameLengths.clear();
				} else {
					cout << "Why the early ENDANIM?" << endl;
				}
			} else if(arg == "KEY"){
				// TODO load all piece normals into keyFrame vectors, then take the cross product of normal n and normal n+1 to get the rotation normal that, along with the initial theta, is stored in Animation.keyFrames
				// next, divide the angle between n and n+1 by keyLength and store it in frameDeltas. Good luck!
				if(currentAnimation != ""){
					int frameLength;
					line >> frameLength;
					curFrameLengths.push_back(frameLength);
					vector<VertexF> keyFrame;
					while(!line.eof()){
						VertexF v;
						line >> v.x;
						if(!line.eof()){
							line >> v.y;
							if(!line.eof()){
								line >> v.z;
							} else {
								cout << filename << ": Error: improper KEY statement." << endl;
							}
						} else {
							cout << filename << ": Error: improper KEY statement." << endl;
						}
						keyFrame.push_back(v);
					}
					curFrames.push_back(keyFrame);
				} else {
					cout << filename << ": Error: No current animation: KEY statement before ANIM statement." << endl;
				}
			} else {
				cout << filename << ": Unknown argument \"" << arg << "\"." << endl;
			}
		}

		fin.close();

		if(!failure){ // we loaded the model successfully
			models[next_unused_ref] = model;
			filenames[filename] = next_unused_ref;
			++next_unused_ref;
			return next_unused_ref - 1;
		} else {
			cout << "Couldn't load model \"" << filename << "\" successfully." << endl;
			return -1;
		}
	}
}

void ModelManager::UnloadModel(ModelRef ref){ // TODO update me for new hierarchy format and animations
	map<ModelRef,Model*>::iterator modelIter = models.find(ref);
	if(modelIter != models.end()){
		Model* m = modelIter->second;
		for(vector<ModelPiece*>::iterator i = m->pieces.begin(); i != m->pieces.end(); ++i){
			assert(*i != NULL);
			delete *i;
		}
	}
	models.erase(modelIter);
	for(map<string,ModelRef>::iterator i = filenames.begin(); i != filenames.end(); ++i){
		if(i->second == ref){
			filenames.erase(i);
			break;
		}
	}
}

void ModelManager::DrawPiece(Model* model, ModelPiece* piece, TextureManager* textureManager, JointState **initials, JointState **vels){
	assert(piece != NULL);
	glPushMatrix();
	if((*initials) != NULL && (*vels) != NULL){
		glTranslatef(piece->joint.x, piece->joint.y, piece->joint.z);
		glRotatef((*vels)->theta, (*vels)->n.x, (*vels)->n.y, (*vels)->n.z);
		glRotatef((*initials)->theta, (*initials)->n.x, (*initials)->n.y, (*initials)->n.z);
		glTranslatef(-piece->joint.x, -piece->joint.y, -piece->joint.z);
		if((*initials)->next != NULL || (*vels)->next != NULL){
			(*initials) = (*initials)->next;
			(*vels) = (*vels)->next;
		}
	}
	if(piece->displayList == 0){ // we haven't created a display list for this piece yet
		piece->displayList = glGenLists(1);
		glNewList(piece->displayList,GL_COMPILE_AND_EXECUTE);
		if(piece->textured){
			glEnable(GL_TEXTURE_2D);
			if(piece->texture != lastTexture){
				//textureManager->BindTexture(piece->texture);
				lastTexture = piece->texture;
			}
		} else {
			glDisable(GL_TEXTURE_2D);
		}
		glColor3f(1,1,1);
		glBegin(GL_TRIANGLES);
		for(vector<Triangle>::iterator tri = piece->triangles.begin(); tri != piece->triangles.end(); tri++){
			for(int i = 0; i < 3; ++i){
				if(piece->textured){
					glTexCoord2f(model->tex_coords[tri->vt[i]].x, model->tex_coords[tri->vt[i]].y);
				}
				glNormal3f(model->normals[tri->vn[i]].x, model->normals[tri->vn[i]].y, model->normals[tri->vn[i]].z);
				glVertex3f(model->vertices[tri->v[i]].x, model->vertices[tri->v[i]].y, model->vertices[tri->v[i]].z);
			}
		}
		glEnd();
		glEndList();
	} else {
		glCallList(piece->displayList);
	}
	for(vector<ModelPiece*>::iterator p = piece->children.begin(); p != piece->children.end(); p++){
		DrawPiece(model,*p,textureManager, initials, vels); // yay recursion :D
	}
	glPopMatrix();
}

void ModelManager::DrawModel(ModelRef ref, TextureManager *textureManager, AnimationInstance *animationInstance){
	JointState *initials, *vels;
	if(animationInstance != NULL && animationInstance->animation != NULL){
		initials = animationInstance->animation->keyFrames[animationInstance->key];
		vels = animationInstance->currentDelta;
	} else {
		initials = NULL;
		vels = NULL;
	}
	lastTexture = -1; // we should always change the texture before drawing
	map<ModelRef,Model*>::iterator modelIter = models.find(ref);
	if(modelIter != models.end()){
		Model* model = modelIter->second;
		for(vector<ModelPiece*>::iterator p = model->pieces.begin(); p != model->pieces.end(); ++p){
			DrawPiece(model,*p,textureManager,&initials,&vels);
		}
	}
}
