#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>

#include <iostream>
using namespace std;
class Sphere: public Object3D
{
public:
	Sphere(){ 
		this->center = Vector3f(0, 0, 0);
		this->radius = 1.0;
	}

	Sphere( Vector3f center , float radius , Material* material ):Object3D(material){
		this->center = center;
		this->radius = radius;
	}
	

	~Sphere(){}

	virtual bool intersect( const Ray& r , Hit& h , float tmin){
		double a = r.getDirection().absSquared();
		double b = 2 * Vector3f::dot(r.getDirection(), (r.getOrigin() - this->center)); 
		double c = (r.getOrigin() - this->center).absSquared() - this->radius * this->radius; 
		double disc = b * b - 4 * a * c;
//		cout<<"ALL a = "<<a<<" b = "<<b<<" c = "<<c<<" disc = "<<disc<<endl;
		double t0 = 0, t1 = 0;
		double t;
		if (disc < 0) return false;
		else if (disc == 0){
			t0 = t1 = -0.5 * b / a;
		} else {
			t0 = 0.5 * (-b + sqrt(disc)) / a;
			t1 = 0.5 * (-b - sqrt(disc)) / a;
			//cout<<"a = "<<a<<" b = "<<b<<" c = "<<c<<" disc = "<<disc<<endl;
			//cout<<"t0 = "<<t0<<" t1 = "<<t1<<endl;
		}
		t = min(t0, t1);
		if (t < 0) t = max(t0, t1);
		if (t < 0) return false;
		if ((float)(t) >= tmin && (float)(t) < h.getT()){
			h.set((float)(t), this->material, (t*r.getDirection() + r.getOrigin() - this->center).normalized());
//			h.set((float)(t), this->material, h.getNormal());
//			cout<<"INSERSECT a = "<<a<<" b = "<<b<<" c = "<<c<<" disc = "<<disc<<" t = "<<t<<endl;
			return true;
		}
		return false; 
	}

protected:
	Vector3f center;
	float radius;

};


#endif
