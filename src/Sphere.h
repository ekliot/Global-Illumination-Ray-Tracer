#include <glm/vec3.hpp>
#include "Object.h"
#include "Material.h"

class Sphere :Object
{
	glm::vec3* point;
	float* radius;

	Sphere(glm::vec3* point, float* radius, Material* mat);
	float intersection(Ray* ray);
	bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
	void transform(mat4 mat);
	vec3 Sphere::extractScale(const mat4 &m);

};