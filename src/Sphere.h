#include <glm/vec3.hpp>
#include "Object.h"

class Sphere :Object
{
	glm::vec3 point;
	float radius;

	Sphere(glm::vec3 point, float radius);
	float intersection(Ray ray);
	

};