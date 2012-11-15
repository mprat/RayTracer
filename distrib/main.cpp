#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
  // Fill in your implementation here.
	int size_x, size_y;
	char* filename; 
	
  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
  for( int argNum = 1; argNum < argc; ++argNum )
    {
      std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
		if (strcmp(argv[argNum], "size") == 0){
			if (argNum + 2 < argc){
				if (isdigit(atoi(argv[argNum + 1]))){
					size_x = atoi(argv[argNum + 1]);
				} else {
					cout<<"size in x must be a number"<<endl;
					return 0;
				}
				if (isdigit(atoi(argv[argNum + 2]))){
					size_y = atoi(argv[argNum + 2]);
				} else {
					cout<<"size in y must be a number"<<endl;
					return 0;
				}
			} else {
				cout<< "Not enough size arguments!"<<endl;
				return 0;
			}
		} else if (strcmp(argv[argNum], "-input") == 0){
			if (argNum + 1 < argc){
				filename = argv[argNum + 1];
			} else {
				cout<<"not enough filename arguments"<<endl;
				return 0;
			}
		}
    }
	
    
  // First, parse the scene using SceneParser.
  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene.  Write the color at the intersection to that
  // pixel in your output image.

	SceneParser parser = SceneParser(filename);
		

 
  ///TODO: below demonstrates how to use the provided Image class
  ///Should be removed when you start
  Vector3f pixelColor (1.0f,0,0);
  //width and height
  Image image( 10 , 15 );
  image.SetPixel( 5,5, pixelColor );
  image.SaveImage("demo.bmp");
  return 0;
}

