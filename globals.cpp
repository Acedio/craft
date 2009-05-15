#include "globals.h"

#include <string>
using namespace std;

string TrimFileName(string filepath){
	return string(filepath,0,filepath.find_last_of('/')+1);
}
