#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <glm/vec3.hpp>
#include "Object.h"
#include "Material.h"

using namespace glm;

class Triangle : Object {
private:
	vec3 a;
	vec3 b;
	vec3 c;

public:
	Triangle( vec3 a, vec3 b, vec3 c, Material mat );
	float intersection( Ray ray );
    float getBarycentricCoords( vec3 p );

};

#endif
