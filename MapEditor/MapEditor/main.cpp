//////////////////////
//////////////////////
///// Map Editor /////
//////////////////////
//////////////////////

#include <iostream>
#include <fstream>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include <SDL/SDL.h>
using namespace std;

int** h_map;

int main (int argc, char **argv)
{	
	if (argc < 2)
	{
		cout << "You need more arguments." << endl;
		return 0;
	}

	fstream filestr;
	filestr.open (argv[1] , fstream::in);
	
	int width, height;

	string temp;

	filestr >> temp;

	if (temp != "P3")
	{
		cout << "Not a valid PPM file." << endl;
		return 0;
	}

	getline(filestr, temp);
	getline(filestr, temp);
	filestr >> width;
	filestr >> height;
	getline(filestr, temp);
	getline(filestr, temp);



	

	h_map = new int* [height];
	
	for (int i = 0; i < height; i++) // initialize the arrays
	{
		h_map[i] = new int [width];
		
		for (int j = 0; j < width; j++)
		{
			int r, g, b, rgb;
			filestr >> r;
			filestr >> g;
			filestr >> b;
			
			rgb = (r << 16) | (g << 8) | (b);

			h_map[i][j] = rgb;

			//cout << r << " " << g << " " << b << endl;
			//cout << rgb << endl;
		}
	}

	filestr.close();

	fstream mapfile;

	mapfile.open ("test.map", fstream::out | fstream::trunc);

	mapfile << width << " " << height << endl;

	for (int i = 0; i < height; i++) // initialize the arrays
	{		
		for (int j = 0; j < width; j++)
		{
			switch (h_map[i][j])
			{
				case 0x000000: mapfile << '.' << " "; // walkable
					break;
				case 0xFFFFFF: mapfile << '#' << " "; // non-walkable
					break;
				case 0xFF0000: mapfile << '1' << " "; // p1 start point
					break;
				case 0x00FFFF: mapfile << '2' << " "; // p2 start point
					break;
				case 0x00FF00: mapfile << '^' << " "; // trees
					break;
				case 0xFFFF00: mapfile << '*' << " "; // gold
					break;
				default: mapfile << '.' << " "; // walkable
					break;
			}
		}
		mapfile << endl;
	}

	mapfile.close();

	for (int i = 0; i < height; i++) // delete arrays
	{
		delete[] h_map[i];
	}
	delete[] h_map;	

	return 0;
}