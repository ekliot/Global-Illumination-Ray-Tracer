#include <glm/vec3.hpp>
#include "Object.h"
#include "Material.h"

class Sphere :Object
{
	glm::vec3 point;
	float radius;

	Sphere(glm::vec3 point, float radius, Material mat);
	float intersection(Ray ray);
	

};