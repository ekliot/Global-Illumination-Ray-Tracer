/**
 *
 */

#if defined(_WIN32) || defined(_WIN64)
#include "stdafx.h"
#include <windows.h>
#endif

#include <iostream>

#include <vector>
#include <math.h>
#include <glm/glm.hpp>


#include <glm/vec3.hpp>


using namespace glm;
#include "Ray.h"

Ray::Ray( vec3* start, vec3* dir ) : origin(start), direction(dir) {
	normalizeRay();
}

void Ray::normalizeRay(){
	//this->direction = normalize(this->direction);
	
    double magnitude = sqrt(this->direction->x * this->direction->x
        + this->direction->y*this->direction->y
        + this->direction->z*this->direction->z);
    std::cout << "magnitude: " << magnitude << '\n';
    *(this->direction) = vec3(this->direction->x / magnitude, this->direction->y / magnitude, this->direction->z / magnitude);
	
}
