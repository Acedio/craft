#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <sstream>
#include <string>
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

		vector<vector<VertexF> > currentAnimationFrames;

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
					cout << "JOINT found but no pieces exist. Are you missing an OBJ statement?" << endl;
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
					cout << "Could not find model piece \"" << pieceName << "\". Joint not set." << endl;
				}
			} else if(arg == "PIECETREE"){
				if(objPieces.empty()){
					cout << "PIECETREE found but no pieces exist. Are you missing an OBJ statement?" << endl;
				}
				stack<ModelPiece*> pieceStack;
				while(!line.eof()){
					string s;
					line >> s;
					if(s == "["){
						if(model->pieces.empty()){ // no pieces defined yet, nothing to push!
							cout << "No pieces defined, cannot push onto piecetree!" << endl;
						} else {
							if(pieceStack.empty()){
								pieceStack.push(model->pieces.back());
							} else {
								pieceStack.push(pieceStack.top()->children.back());
							}
						}
					} else if(s == "]"){
						if(pieceStack.empty()){
							cout << "Unexpected ']': piecestack empty." << endl;
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
							cout << "Could not find model piece \"" << s << "\". Piecetree may be broken." << endl;
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
					cout << "Error: piecestack not empty. Missing ']'?" << endl;
				}
			} else if(arg == "ANIM"){
				if(line.eof()){
					cout << "No animation name specified!" << endl;
				} else {
					if(currentAnimation != ""){
						// TODO do calculations for frame interpolation here (and once afterwards.)
					}
					line >> currentAnimation;
				}
			} else if(arg == "KEY"){
				// TODO load all piece normals into keyFrame vectors, then take the cross product of normal n and normal n+1 to get the rotation normal that, along with the initial theta, is stored in Animation.keyFrames
				// next, divide the angle between n and n+1 by keyLength and store it in frameDeltas. Good luck!
				if(currentAnimation != ""){
					int frameLength;
					line >> frameLength;
					model->animations[currentAnimation]->keyLengths.push_back(frameLength);
					vector<VertexF> keyFrame;
					while(!line.eof()){
						VertexF v;
						line >> v.x >> v.y >> v.z;
						keyFrame.push_back(v);
					}
					currentAnimationFrames.push_back(keyFrame);
				} else {
					cout << "No current animation: KEY statement before ANIM statement." << endl;
				}
			} else {
				cout << "Unknown argument \"" << arg << "\"." << endl;
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

void ModelManager::UnloadModel(ModelRef ref){ // TODO update me for new hierarchy format
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

void ModelManager::DrawPiece(Model* model, ModelPiece* piece, TextureManager* textureManager){
	assert(piece != NULL);
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
		DrawPiece(model,*p,textureManager); // yay recursion :D
	}
}

void ModelManager::DrawModel(ModelRef ref, TextureManager* textureManager){
	lastTexture = -1; // we should always change the texture before drawing
	map<ModelRef,Model*>::iterator modelIter = models.find(ref);
	if(modelIter != models.end()){
		Model* model = modelIter->second;
		for(vector<ModelPiece*>::iterator p = model->pieces.begin(); p != model->pieces.end(); ++p){
			DrawPiece(model,*p,textureManager);
		}
	}
}
