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
	return incoming - 2.0*Vector3f::dot(incoming, normal) * normal;
}

bool transmittedDirection( const Vector3f& normal, const Vector3f& incoming, 
        float index_n, float index_nt, 
        Vector3f& transmitted)
{
	float undersq = 1.0 - (index_n*index_n*(1.0 - (Vector3f::dot(incoming, normal) * Vector3f::dot(incoming, normal))))/(index_nt*index_nt);
	if (undersq < 0) return false;
	transmitted = index_n / index_nt *(incoming - normal*Vector3f::dot(incoming, normal)) - normal*sqrt(undersq);
	transmitted = transmitted.normalized();
	return true;	
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
				float disttolight;	
				m_scene->getLight(k)->getIllumination(hitPt, dir, col, disttolight);
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
						finalColor += hit.getMaterial()->Shade(ray, hit, dir, col);
					}
				} else {
					finalColor += hit.getMaterial()->Shade(ray, hit, dir, col);
				}

			}
		
			//Fresnel
			float n = refr_index;
			float n_t = hit.getMaterial()->getRefractionIndex();
			float r_0 = 0;
			float r = 1.0;
			float c = 0;
			r_0 = pow((n_t - n)/(n_t + n), 2);
			//if (n_t + n == 0) r_0 = 1;
			c = abs(Vector3f::dot(ray.getDirection(), hit.getNormal()));
			r = r_0 + (1.0 - r_0)*pow(1.0 - c, 5);			

			//reflection
			Vector3f reflectContribution(0, 0, 0);
			if (bounces > 0 && hit.getMaterial()->getSpecularColor() != Vector3f(0, 0, 0))
			{
				Vector3f mirrorDir = mirrorDirection(hit.getNormal(), ray.getDirection());
				Ray mirrorRay(hitPt, mirrorDir);
				Hit h;
				//reflectContribution += traceRay(mirrorRay, EPSILON, bounces - 1, hit.getMaterial()->getRefractionIndex(), h) * hit.getMaterial()->getSpecularColor();
				if (n_t + n == 0) r = pow(1.0 - c, 5);
				if ((r != 1) && !(r < 0)) cout<<"n = "<<n<<" n_t = "<<n_t<<" r_0 = "<<r_0<<" r = "<<r<<endl;
				finalColor += r*traceRay(mirrorRay, EPSILON, bounces - 1, hit.getMaterial()->getRefractionIndex(), h) * hit.getMaterial()->getSpecularColor();
			}

			//refraction
			Vector3f refractContribution(0, 0, 0);
			if (bounces > 0 && hit.getMaterial()->getRefractionIndex() > 0){
				Vector3f transmitted;
				if (n > n_t){
					c = abs(Vector3f::dot(transmitted, hit.getNormal()));
					r = r_0 + (1.0 - r_0)*pow(1.0 - c, 5);
				}
				if (n == hit.getMaterial()->getRefractionIndex()){
					n_t = 1.0;
					r_0 = pow((n_t - n)/(n_t + n), 2);
					r = r_0 + (1.0 - r_0)*pow(1.0 - c, 5);			
				}
				if (transmittedDirection(hit.getNormal(), ray.getDirection(), n, n_t, transmitted)){
					Hit h;
					Ray refractRay(hitPt, transmitted);
					//refractContribution += traceRay(refractRay, EPSILON, bounces - 1, hit.getMaterial()->getRefractionIndex(), h) * hit.getMaterial()->getSpecularColor();
					//if ((r != 1) && !(r < 0)) cout<<"n = "<<n<<" n_t = "<<n_t<<" r_0 = "<<r_0<<" r = "<<r<<endl;
					finalColor += (1.0 - r)*traceRay(refractRay, EPSILON, bounces - 1, hit.getMaterial()->getRefractionIndex(), h) * hit.getMaterial()->getSpecularColor();
				}
			}					
		
//			finalColor += r*reflectContribution;
//			finalColor += (1.0 - r) * refractContribution;

		} else {
			finalColor = m_scene->getBackgroundColor(ray.getDirection());
		}

		return finalColor;
	} else {
		return Vector3f(0, 0, 0); 
	}
}

