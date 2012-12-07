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
	bool filter = false;

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
		} else if (strcmp(argv[argNum], "-filter") == 0){
			filter = true;
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

		
		if (filter){
			Image finalfilter(size_x * 3, size_y *3);
			float k0 = .1201;
			float k1 = .2339;
			float k2 = .2931;
			float k3 = .2339;
			float k4 = .1201;
			Vector3f colorpix(0, 0, 0);
			//horizontal blur
			for (int i = 0; i < size_x*3; i++){
				for (int j = 0; j < size_y * 3; j++){
					colorpix = k0*final.GetPixel(i, max(j - 2, 0)) + k1*final.GetPixel(i, max(j - 1, 0)) + k2*final.GetPixel(i, j) + k3*final.GetPixel(i, min(j+1, final.Width() - 1)) + k4*final.GetPixel(i, min(j+2, final.Width() - 1));
					finalfilter.SetPixel(i, j, colorpix);
				}
			}

			colorpix = Vector3f(0, 0, 0);
			//vertical blur
			Image finalAfterVert(size_x*3, size_y*3);
			for (int i = 0; i < size_x*3; i++){
				for (int j = 0; j < size_y*3; j++){
					colorpix = k0*finalfilter.GetPixel(max(i - 2, 0), j) + k1*finalfilter.GetPixel(max(i - 1, 0), j) + k2*finalfilter.GetPixel(i, j) + k3*finalfilter.GetPixel(min(i+1, finalfilter.Height() - 1), j) + k4*finalfilter.GetPixel(min(i+2, finalfilter.Height() - 1), j);
					finalAfterVert.SetPixel(i, j, colorpix);
				}
			}

			//downsample
			Image finalOrig(size_x, size_y);
			Vector3f downColor(0, 0, 0);
			for (int i = 0; i < size_x; i++){
				for (int j = 0; j < size_y; j++){
					//cout<<"i = "<<i<<" j = "<<j<<endl;
					//cout<<"1 = "<<finalAfterVert.GetPixel(max(3*i - 1, 0), max(3*j - 1, 0))<<endl;
					//cout<<"2 = "<<finalAfterVert.GetPixel(3*i, max(3*j - 1, 0))<<endl;
					//cout<<"3 = "<<finalAfterVert.GetPixel(min(3*i + 1, finalAfterVert.Height() - 1), max(3*j - 1, 0))<<endl;
					//cout<<"4 = "<<finalAfterVert.GetPixel(max(3*i - 1, 0), 3*j)<<endl;
					//cout<<"5 = "<<finalAfterVert.GetPixel(3*i, 3*j)<<endl;
					//cout<<"6 = "<<finalAfterVert.GetPixel(min(3*i + 1, finalAfterVert.Height() - 1), 3*j)<<endl;
					//cout<<"7 = "<<finalAfterVert.GetPixel(max(3*i - 1, 0), min(3*j + 1, finalAfterVert.Width() - 1))<<endl;
					//cout<<"8 = "<<finalAfterVert.GetPixel(3*i, max(3*j + 1, finalAfterVert.Width() - 1))<<endl;
					//cout<<"9 = "<<finalAfterVert.GetPixel(max(3*i + 1, finalAfterVert.Height() - 1), max(3*j + 1, finalAfterVert.Height() - 1))<<endl;
					downColor = (finalAfterVert.GetPixel(max(3*i - 1, 0), max(3*j - 1, 0)) + finalAfterVert.GetPixel(3*i, max(3*j - 1, 0)) + finalAfterVert.GetPixel(min(3*i + 1, finalAfterVert.Height() - 1), max(3*j - 1, 0)) + finalAfterVert.GetPixel(max(3*i - 1, 0), 3*j) + finalAfterVert.GetPixel(3*i, 3*j) + finalAfterVert.GetPixel(min(3*i + 1, finalAfterVert.Height() - 1), 3*j) + finalAfterVert.GetPixel(max(3*i - 1, 0), min(3*j + 1, finalAfterVert.Width() - 1)) + finalAfterVert.GetPixel(3*i, min(3*j + 1, finalAfterVert.Width() - 1)) + finalAfterVert.GetPixel(min(3*i + 1, finalAfterVert.Height() - 1), min(3*j + 1, finalAfterVert.Height() - 1)))/9;
					finalOrig.SetPixel(i, j, downColor);
				}
			}	

			finalOrig.SaveImage(outputfilename);
		} else {
			final.SaveImage(outputfilename);
		}
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

