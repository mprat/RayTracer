#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include <vecmath.h>

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"

class Material
{
public:
	
 Material( const Vector3f& d_color ,const Vector3f& s_color=Vector3f::ZERO, float s=0):
  diffuseColor( d_color),specularColor(s_color), shininess(s)
  {
        	
  }

  virtual ~Material()
    {

    }

  virtual Vector3f getDiffuseColor() const 
  { 
    return  diffuseColor;
  }
    

  Vector3f Shade( const Ray& ray, const Hit& hit,
                  const Vector3f& dirToLight, const Vector3f& lightColor ) {

    float d = max(0.0f, Vector3f::dot(dirToLight, hit.getNormal()));
	Vector3f R = ray.getDirection() - 2.0 * Vector3f::dot(ray.getDirection(), hit.getNormal()) * hit.getNormal();
	float c_s = max(0.0f, Vector3f::dot(dirToLight, R));
	if (t.valid()){
		return d * diffuseColor * t(hit.texCoord.x(), hit.texCoord.y()) + pow(c_s, shininess) * lightColor * specularColor; 	
	} else {
		return d * diffuseColor * lightColor + pow(c_s, shininess) * lightColor * specularColor;
	}	
  }

  void loadTexture(const char * filename){
    t.load(filename);
  }
	
  bool validTexture(){
		return t.valid();
	}
 protected:
  Vector3f diffuseColor;
  Vector3f specularColor;
  float shininess;
  Texture t;
};

#endif // MATERIAL_H
