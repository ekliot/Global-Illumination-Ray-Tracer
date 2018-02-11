#include "stdafx.h"

#include "../../stdafx.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include <glm/vec3.hpp>
#include "Object.h"

#include "Sphere.h"

		Sphere::Sphere(glm::vec3 point, float radius, Material mat):Object(mat){
			this->point = point;
			this->radius = radius;
		}

		float Sphere::intersection(Ray ray)
		{
			// do this later
			return -1;
		}