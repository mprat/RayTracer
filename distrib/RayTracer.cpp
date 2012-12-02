#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

//IMPLEMENT THESE FUNCTIONS
//These function definitions are mere suggestions. Change them as you like.
Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces 
  //more arguments if you need...
                      ) :
  m_scene(scene)

{
  g=scene->getGroup();
  m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit& hit ) const
{
	Vector3f finalColor(0, 0, 0);
	bool intersect = g->intersect(ray, hit, tmin); 
	if (intersect){
		Vector3f dir;
		Vector3f col;
		if (hit.getMaterial()->validTexture()) {
			finalColor = m_scene->getAmbientLight() * hit.getMaterial()->returnTexture(hit);
		} else {
			finalColor = m_scene->getAmbientLight() * hit.getMaterial()->getDiffuseColor();
		}
		for (int k = 0; k < m_scene->getNumLights(); k++){
			float disttolight = hit.getT();
			m_scene->getLight(k)->getIllumination(hit.getT()*ray.getDirection(), dir, col, disttolight);
			finalColor += hit.getMaterial()->Shade(ray, hit, dir, col);
		}
	} else {
		finalColor = m_scene->getBackgroundColor(ray.getDirection());
	}


    return finalColor;
}
