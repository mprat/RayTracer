#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include <vecmath.h>
#include <float.h>
#include <cmath>


class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vector2f& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}
protected:
//	Vector3f center; 
//	Vector3f direction;
//	Vector3f up;
//	Vector3f horizontal;
	Vector3f u;
	Vector3f v;
	Vector3f w;
	Vector3f center;
	float angle;
	double D;
};

class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vector3f& center, const Vector3f& direction,const Vector3f& up , float angle){
		this->center = center;
		this->w = direction;
		this->u = (Vector3f::cross(this->w, up)).normalized();
		this->v = (Vector3f::cross(this->u, this->w)).normalized();
		this->angle = angle;
		this->D = 1 / tan(angle / 2) ;
	}

	virtual Ray generateRay( const Vector2f& point){
		return Ray(this->center, (point.x() * this->u + point.y()*this->v + D * this->w).normalized());	
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:

};

#endif //CAMERA_H
