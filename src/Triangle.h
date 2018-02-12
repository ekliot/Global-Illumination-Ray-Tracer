#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <glm/vec3.hpp>
#include "Object.h"
#include "Material.h"
#include <glm/matrix.hpp>

using namespace glm;

class Triangle :Object
{

	vec3 a;
	vec3 b;
	vec3 c;

	public:
		Triangle(vec3 a, vec3 b, vec3 c, Material mat);
		float intersection(Ray ray);
		void transform(mat4 matrix);
		float getBarycentricCoords(vec3 p);
		float intersectPlane(Ray ray);

};
#endif