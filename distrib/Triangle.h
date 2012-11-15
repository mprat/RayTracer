#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include <vecmath.h>
#include <cmath>
#include <iostream>

using namespace std;
class Triangle: public Object3D
{
public:
	Triangle();
        ///@param a b c are three vertex positions of the triangle
	Triangle( const Vector3f& a, const Vector3f& b, const Vector3f& c, Material* m):Object3D(m){
          hasTex = false;
		this->a = a;
		this->b = b;
		this->c = c;
	}

	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin){
		Matrix3f A(a - b, a - c, ray.getDirection(), true);
		Vector3f col = a - ray.getOrigin();	
		float detA = A.determinant();
		Matrix3f A_beta(A);
		A_beta.setCol(0, col);
		float beta = A_beta.determinant() / detA;
		Matrix3f A_gamma(A);
		A_gamma.setCol(1, col);
		float gamma = A_gamma.determinant() / detA;
		Matrix3f A_t(A);
		A_t.setCol(2, col);
		float t = A_t.determinant() / detA;
		if (t >= tmin && t < hit.getT() && beta + gamma <= 1.0 && beta >= 0 && gamma >= 0){
			hit.set(t, this->material, ((1 - beta - gamma) *normals[0] + beta *normals[1] + gamma * normals[2]).normalized());
			return true;
		}	
		return false;
	}
	bool hasTex;
	Vector3f normals[3];
	Vector2f texCoords[3];
protected:
	Vector3f a;
	Vector3f b;
	Vector3f c;
};

#endif //TRIANGLE_H
