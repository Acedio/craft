#include <fstream>
#include <vector>
#include <stack>
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
	for(map<ModelRef,Model*>::iterator i = models.begin(); i != models.end(); ++i){
		assert(i->second != NULL);
		delete i->second;
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
			stack<ModelPiece*> pieceStack;
			while(!line.eof()){
				string s;
				line >> s;
				if(s == "["){
					if(model->pieces.empty()){ // no pieces defined yet, nothing to push!
						cout << "No pieces defined, cannot push onto piecetree!" << endl;
					} else {
						pieceStack.push(model->pieces.back());
					}
				} else if(s == "]"){
					if(pieceStack.empty()){
						cout << "Unexpected ']': piecestack empty." << endl;
					} else {
						pieceStack.pop();
					}
				} else {
					VertexF joint;
					line >> joint.x >> joint.y >> joint.z;
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

void ModelManager::UnloadModel(ModelRef ref){
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

void ModelManager::DrawModel(ModelRef ref, TextureManager* textureManager){
	map<ModelRef,Model*>::iterator modelIter = models.find(ref);
	if(modelIter != models.end()){
		Model* m = modelIter->second;
		for(vector<ModelPiece*>::iterator pi = m->pieces.begin(); pi != m->pieces.end(); ++pi){
			ModelPiece* p = *pi;
			assert(p != NULL);
			if(p->textured){
				glEnable(GL_TEXTURE_2D);
				textureManager->BindTexture(p->texture);
			} else {
				glDisable(GL_TEXTURE_2D);
			}
			glColor3f(1,1,1);
			glBegin(GL_TRIANGLES);
			for(vector<Triangle>::iterator tri = p->triangles.begin(); tri != p->triangles.end(); tri++){
				for(int i = 0; i < 3; ++i){
					if(p->textured){
						glTexCoord2f(m->tex_coords[tri->vt[i]].x, m->tex_coords[tri->vt[i]].y);
					}
					glNormal3f(m->normals[tri->vn[i]].x, m->normals[tri->vn[i]].y, m->normals[tri->vn[i]].z);
					glVertex3f(m->vertices[tri->v[i]].x, m->vertices[tri->v[i]].y, m->vertices[tri->v[i]].z);
				}
			}
			glEnd();
		}
	}
}
