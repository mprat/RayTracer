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
#include "RayTracer.h"

using namespace std;

float clampedDepth ( float depthInput, float depthMin , float depthMax);

#include "bitmap_image.hpp"
int main( int argc, char* argv[] )
{
  // Fill in your implementation here.
	int size_x = 0, size_y = 0;
	char* filename = ""; 
	char* outputfilename = "";
	int max_bounces = 0;
	bool shadows = false;
	//double aspect = 0.0;	
	bool jitter = false;

  // This loop loops over each of the input arguments.
  // argNum is initialized to 1 because the first
  // "argument" provided to the program is actually the
  // name of the executable (in our case, "a4").
  for( int argNum = 1; argNum < argc; ++argNum )
    {
      std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
		if (strcmp(argv[argNum], "-size") == 0){
			if (argNum + 2 < argc){
				//TODO: check if number
				//if (isnumber(atoi(argv[argNum + 1]))){
					size_x = atoi(argv[argNum + 1]);
				//} else {
				//	cout<<"size in x must be a number"<<endl;
				//	return 0;
				//}
				//if (isdigit(atoi(argv[argNum + 2]))){
					size_y = atoi(argv[argNum + 2]);
				//} else {
				//	cout<<"size in y must be a number"<<endl;
				//	return 0;
				//}
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
		} else if (strcmp(argv[argNum], "-output") == 0){
			if (argNum + 1 < argc){
				outputfilename = argv[argNum + 1];
			} else {
				cout<<"not enough output filename arguments"<<endl;
				return 0;
			}
		} else if (strcmp(argv[argNum], "-bounces") == 0){
			if (argNum + 1 < argc){
				max_bounces = atoi(argv[argNum + 1]);
			} else {
				cout<<"not enough bounce arguments"<<endl;
				return 0;
			}
		} else if (strcmp(argv[argNum], "-shadows") == 0){
			shadows = true;
		} else if (strcmp(argv[argNum], "-jitter") == 0){
			jitter = true;
		}
    }
	
	//aspect = size_x / size_y;
    
  // First, parse the scene using SceneParser.
  // Then loop over each pixel in the image, shooting a ray
  // through that pixel and finding its intersection with
  // the scene.  Write the color at the intersection to that
  // pixel in your output image.

	SceneParser *parser = new SceneParser(filename);
	RayTracer raytracer(parser, max_bounces, shadows);
		//TODO: add aspect ratio
	cout<<parser->getGroup()->getGroupSize()<<endl;
	if (jitter){
		float step_x = 2.0f / float(size_x * 3);
		float step_y = 2.0f / float(size_y * 3);

		float r_i = (float)rand()/(float)RAND_MAX - 0.5;
		float r_j = (float)rand()/(float)RAND_MAX - 0.5;		

		Image final(size_x*3, size_y*3);
		for (int i = 0; i < size_x*3; i++){
			for (int j = 0; j < size_y*3; j++){
				float itogen = (float)(i + r_i) * step_x - 1.0f;
				float jtogen = (float)(j + r_j) * step_y - 1.0f;
				if (itogen > 1) itogen = 1.0;
				if (jtogen > 1) jtogen = 1.0;
				if (itogen < -1) itogen = -1.0;
				if (jtogen < -1) jtogen = -1.0;
				Ray r = parser->getCamera()->generateRay((Vector2f(itogen, jtogen)));
				Hit h;
				Vector3f color = raytracer.traceRay(r, parser->getCamera()->getTMin(), max_bounces, 1.0, h);  
				final.SetPixel(i, j, color);
				
			}
		}

		final.SaveImage(outputfilename);
	} else {
		float step_x = 2.0f / float(size_x);
		float step_y = 2.0f / float(size_y);

		Image final(size_x, size_y);
		for (int i = 0; i < size_x; i++){
			for (int j = 0; j < size_y; j++){
				Ray r = parser->getCamera()->generateRay((Vector2f((float)(i) * step_x - 1.0f, (float)(j) * step_y - 1.0f)));
				Hit h;
				Vector3f color = raytracer.traceRay(r, parser->getCamera()->getTMin(), max_bounces, 1.0, h);  
				final.SetPixel(i, j, color);
				
			}
		}

		final.SaveImage(outputfilename);
	}	

  return 0;
}

