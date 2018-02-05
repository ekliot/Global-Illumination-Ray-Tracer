
#ifdef _WIN32
#include "stdafx.h"
#include <windows.h>
#endif

#include <glm/vec3.hpp>
#include "Object.h"

class Sphere :Object
{
		glm::vec3 point;
		float radius;

		Sphere(glm::vec3 point, float radius){
			this->point = point;
			this->radius = radius;
		}

		float intersection(Ray ray)
		{
			// do this later
			return -1;
		}


};