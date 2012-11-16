#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vecmath.h>
#include "Object3D.h"
#include "VecUtils.h"
///TODO implement this class
///So that the intersect function first transforms the ray
///Add more fields as necessary
class Transform: public Object3D
{
public: 
  Transform(){}
 Transform( const Matrix4f& m, Object3D* obj ):o(obj){
    this->transform = m;
  }
  ~Transform(){
  }
  virtual bool intersect( const Ray& r , Hit& h , float tmin){
    Ray trans_r(VecUtils::transformPoint(transform.inverse(), r.getOrigin()), VecUtils::transformDirection(transform.inverse(), r.getDirection()));
	bool inter = o->intersect( trans_r , h , tmin);
	if (inter) {
		h.setNormal((transform.getSubmatrix3x3(0, 0).inverse().transposed() * h.getNormal()).normalized());
	}
	return inter;
  }

 protected:
  Object3D* o; //un-transformed object	
	Matrix4f transform;
};

#endif //TRANSFORM_H
