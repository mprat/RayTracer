#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"

#define EPSILON 0.001

Vector3f mirrorDirection( const Vector3f& normal, const Vector3f& incoming)
{
	return incoming - 2*Material::pointwiseDot(incoming, normal) * normal;
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
}

RayTracer::RayTracer( SceneParser * scene, int max_bounces, bool shadows) :
  m_scene(scene)

{
  g=scene->getGroup();
  m_maxBounces = max_bounces;
  m_shadows = shadows;
}

RayTracer::~RayTracer()
{
}

Vector3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit& hit) 
{
	if (bounces >= 0){
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
			
			Vector3f hitPt = ray.pointAtParameter(hit.getT());
			for (int k = 0; k < m_scene->getNumLights(); k++){
				float disttolight = m_scene->getLight(k)->distanceToLight(hitPt);	
				if (m_shadows){
					//cast shadow ray
					Hit hitShadow(disttolight, NULL, NULL);
					Ray rayShadow(hitPt, m_scene->getLight(k)->directionToLight(hitPt));
					for (int objIndex = 0; objIndex < g->getGroupSize(); objIndex++){
						g->getObject(objIndex)->intersect(rayShadow, hitShadow, EPSILON); 
					}
					//endcastshadow ray
					if (hitShadow.getT() == disttolight){
						//cout<<"hit2t = "<<hit2.getT()<<"  disttolight = "<<disttolight<<endl;
						m_scene->getLight(k)->getIllumination(ray.pointAtParameter(hit.getT()), dir, col, disttolight);
						finalColor += hit.getMaterial()->Shade(ray, hit, dir, col);
					}
				} else {
					m_scene->getLight(k)->getIllumination(hit.getT()*ray.getDirection(), dir, col, disttolight);
					finalColor += hit.getMaterial()->Shade(ray, hit, dir, col);
				}

			}
			
			//mirror reflection
			if (bounces > 0 && hit.getMaterial()->getSpecularColor() != Vector3f(0, 0, 0))
			{
				Vector3f mirrorDir = mirrorDirection(hit.getNormal(), ray.getDirection());
				Ray mirrorRay(hitPt, mirrorDir);
				Hit h;
				finalColor += traceRay(mirrorRay, EPSILON, bounces - 1, refr_index, h) * h.getMaterial()->getSpecularColor();
			}

			//TODO: transparent
		} else {
			finalColor = m_scene->getBackgroundColor(ray.getDirection());
		}

		return finalColor;
	} else {
		return Vector3f(0, 0, 0); 
	}
}

