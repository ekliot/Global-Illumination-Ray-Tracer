#include <glm/vec3.hpp>
#include "Object.h"

using namespace glm;

class Triangle :Object
{

	vec3 a;
	vec3 b;
	vec3 c;

	public:
		Triangle(vec3 a, vec3 b, vec3 c);
		float intersection(Ray ray);


};