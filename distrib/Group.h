#ifndef GROUP_H
#define GROUP_H


#include "Object3D.h"
#include "Ray.h"
#include "Hit.h"
#include <iostream>

using  namespace std;

class Group:public Object3D
{
public:

  Group(){
	this->num_obj = 0;
  }
	
  Group( int num_objects ){
	this->num_obj = num_objects;
	this->objects.resize(num_objects);
	}

  ~Group(){
   
  }

  virtual bool intersect( const Ray& r , Hit& h , float tmin ) {
		bool inter = false;
		for (unsigned i = 0; i < this->objects.size(); i++){
			bool intereach = this->objects[i]->intersect(r, h, tmin);
			if (intereach) inter = true;
		}
		return inter;
   }


	
  void addObject( int index , Object3D* obj ){
	this->objects[index] = obj;
	}

  int getGroupSize(){ 
	return num_obj;
  }

	Object3D* getObject(int index){
		return this->objects[index];
	}

 private:
	vector<Object3D*> objects;
	int num_obj;
};

#endif
	
