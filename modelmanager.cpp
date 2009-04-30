#include <fstream>
#include <vector>
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

ModelPiece::ModelPiece(string n){
	name = n;
	textured = false;
}

void ModelPiece::Draw(){
	if(textured){
		glEnable(GL_TEXTURE_2D);
	} else {
		glDisable(GL_TEXTURE_2D);
	}
	glBegin(GL_TRIANGLES);
		for(vector<Triangle>::iterator tri = triangles.begin(); tri != triangles.end(); tri++){
			for(int i = 0; i < 3; ++i){
				if(textured){
					glTexCoord2f(tex_coords[tri->vt[i]].x, tex_coords[tri->vt[i]].y);
				}
				glNormal3f(normals[tri->vn[i]].x, normals[tri->vn[i]].y, normals[tri->vn[i]].z);
				glVertex3f(vertices[tri->v[i]].x, vertices[tri->v[i]].y, vertices[tri->v[i]].z);
			}
		}
	glEnd();
}

Model::~Model(){
	for(vector<ModelPiece*>::iterator i = pieces.begin(); i != pieces.end(); ++i){
		assert(*i != NULL);
		delete *i;
	}
}

void Model::Draw(){
	for(vector<ModelPiece*>::iterator i = pieces.begin(); i != pieces.end(); ++i){
		assert(*i != NULL);
		(*i)->Draw();
	}
}

int Model::Load(string filename){
	ifstream fin;
	fin.open(filename.c_str());
	if(fin.fail()){
		cout << "Could not open model file \"" + filename + "\"." << endl;
		return -1;
	}
	
	string s;

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
			string name;
			getline(line,name,' ');
			piece = new ModelPiece(name);
			cout << "Loading ModelPiece \"" << name << "\"." << endl;
		} else if(arg == "v"){
			if(piece != NULL){
				VertexF v;
				line >> v.x;
				line >> v.y;
				line >> v.z;
				piece->vertices.push_back(v);
			} else {
				cout << "Piece accessed before first object named!" << endl;
				delete piece;
				fin.close();
				return -1;
			}
		} else if(arg == "vt"){
			if(piece != NULL){
				piece->textured = true;
				PointF vt;
				line >> vt.x;
				line >> vt.y;
				piece->tex_coords.push_back(vt);
			} else {
				cout << "Piece accessed before first object named!" << endl;
				delete piece;
				fin.close();
				return -1;
			}
		} else if(arg == "vn"){
			if(piece != NULL){
				VertexF vn;
				line >> vn.x;
				line >> vn.y;
				line >> vn.z;
				piece->normals.push_back(vn);
			} else {
				cout << "Piece accessed before first object named!" << endl;
				delete piece;
				fin.close();
				return -1;
			}
		} else if(arg == "f"){
			if(piece != NULL){
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
			} else {
				cout << "Piece accessed before first object named!" << endl;
				delete piece;
				fin.close();
				return -1;
			}
		}
	}
	if(piece != NULL){
		pieces.push_back(piece);
	}

	fin.close();

	return 0;
}

ModelManager::ModelManager(){
	next_unused_ref = 0;
}

ModelManager::~ModelManager(){
	for(map<ModelRef,Model*>::iterator i = models.begin(); i != models.end(); ++i){
		assert(i->second != NULL);
		delete i->second;
	}
}

ModelRef ModelManager::LoadModel(string filename){
	Model* model = new Model();
	if(model->Load(filename) >= 0){ // we loaded the model successfully
		models[next_unused_ref] = model;
		filenames[filename] = next_unused_ref;
		++next_unused_ref;
		return next_unused_ref - 1;
	} else {
		cout << "Couldn't load model \"" << filename << "\" successfully." << endl;
		return 0;
	}
}
