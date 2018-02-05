
#ifdef _WIN32
#include "stdafx.h"
#include <windows.h>
#endif
#include <vector>
#include <glm/vec3.hpp>

using namespace glm;

class Ray
{
	vec3 start;
	vec3 direction;	
	

	Ray(vec3 start, vec3 direction){
		this->start = start;
		this->direction = direction;
		normalize();
	}

	void normalize(){
		double magnitude = sqrt(direction.x*direction.x 
			+ direction.y*direction.y 
			+ direction.z*direction.z);
		direction = vec3(direction.x / magnitude, direction.y / magnitude, direction.z / magnitude);
	}	
	
};