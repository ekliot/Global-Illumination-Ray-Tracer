
//#include "stdafx.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <vector>
#include <glm/vec3.hpp>

using namespace glm;
#include "Ray.h"

	Ray::Ray(vec3 start, vec3 direction){
		this->point = start;
		this->direction = direction;
		normalize();
	}

	void Ray::normalize(){
		double magnitude = sqrt(direction.x * direction.x 
			+ direction.y*direction.y 
			+ direction.z*direction.z);
		direction = vec3(direction.x / magnitude, direction.y / magnitude, direction.z / magnitude);
	}	
	
