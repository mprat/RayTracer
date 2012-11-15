#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

///TODO: 
///Implement Group
///Add data structure to store a list of Object* 
class Group:public Object3D
{
public:

  Group(){
	this->num_obj = 0;
  }
	
  Group( int num_objects ){
	this->num_obj = num_objects;
  }

  ~Group(){
   
  }

  virtual bool intersect( const Ray& r , Hit& h , float tmin ) {
		bool inter = false;
		for (int i = 0; i < objects.size(); i++){
			inter = objects[i].intersect(r, h, tmin);
		}
		return inter;
   }
	
  void addObject( int index , Object3D* obj ){
	objects.push_back(obj);
  }

  int getGroupSize(){ 
	return num_obj;
  }

 private:
	vector<Object3D*> objects;
	int num_obj;
};

#endif
	
