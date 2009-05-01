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

	ModelPiece* piece = NULL;

	while(!getline(fin,s).eof()){
		stringstream line(s,stringstream::in | stringstream::out);
		string arg;
		getline(line,arg,' ');
		if(arg == "#"){
		} else if(arg == "o"){
			if(piece != NULL){
				model->pieces.push_back(piece);
			}
			string name;
			getline(line,name,' ');
			piece = new ModelPiece;
			piece->name = name;
			piece->textured = false;
			cout << "Loading ModelPiece \"" << name << "\"." << endl;
		} else if(arg == "v"){
			if(piece != NULL){
				VertexF v;
				line >> v.x;
				line >> v.y;
				line >> v.z;
				model->vertices.push_back(v);
			} else {
				cout << "Piece accessed before first object named!" << endl;
				failure = true;
				break;
			}
		} else if(arg == "vt"){
			if(piece != NULL){
				piece->textured = true;
				PointF vt;
				line >> vt.x;
				line >> vt.y;
				model->tex_coords.push_back(vt);
			} else {
				cout << "Piece accessed before first object named!" << endl;
				failure = true;
				break;
			}
		} else if(arg == "vn"){
			if(piece != NULL){
				VertexF vn;
				line >> vn.x;
				line >> vn.y;
				line >> vn.z;
				model->normals.push_back(vn);
			} else {
				cout << "Piece accessed before first object named!" << endl;
				failure = true;
				break;
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
				failure = true; 
				break;
			}
		} else if(arg == "usemtl"){
			if(piece != NULL){
				string texfile;
				getline(line,texfile);
				piece->texture = textureManager->LoadTexture(texfile);
			} else {
				cout << "Piece accessed before first object named!" << endl;
				fin.close();
				failure = true; 
				break;
			}
		}
	}
	if(piece != NULL){
		model->pieces.push_back(piece);
	}

	fin.close();

	if(!failure){ // we loaded the model successfully
		models[next_unused_ref] = model;
		filenames[filename] = next_unused_ref;
		++next_unused_ref;
		return next_unused_ref - 1;
	} else {
		cout << "Couldn't load model \"" << filename << "\" successfully." << endl;
		return 0;
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
