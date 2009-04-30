#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

#include "globals.h"
#include "modelmanager.h"

ModelPiece::ModelPiece(string n){
	name = n;
}

Model::~Model(){
	for(vector<ModelPiece*>::iterator i = pieces.begin(); i != pieces.end(); ++i){
		assert(*i != NULL);
		delete *i;
	}
}

void Model::Load(const char* filename){
	ifstream fin(filename);
	
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
		} else if(arg == "v"){
			if(piece != NULL){
				VertexF v;
				line >> v.x;
				line >> v.y;
				line >> v.z;
				piece->vertices.push_back(v);
			} else {
				cout << "Piece accessed before first object named!" << endl;
			}
		} else if(arg == "vt"){
			if(piece != NULL){
				PointF vt;
				line >> vt.x;
				line >> vt.y;
				piece->tex_coords.push_back(vt);
			} else {
				cout << "Piece accessed before first object named!" << endl;
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
					s << temp << " ";
					temp.clear();
					getline(line,temp,' ');
					s << temp << " ";
					temp.clear();
					s >> t.v[i] >> t.vt[i] >> t.vn[i];
				}
				piece->triangles.push_back(t);
			} else {
				cout << "Piece accessed before first object named!" << endl;
			}
		}
	}
	if(piece != NULL){
		pieces.push_back(piece);
	}

	fin.close();
}
