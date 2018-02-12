//#include "stdafx.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include "Object.h"
#include "Ray.h"
#include "Material.h"
#include <glm\matrix.hpp>
#include "Object.h"

using namespace glm;


Object::Object(Material material)
{
	this->material = material;
}

float Object::intersection(Ray ray) { return -1; };
		
void Object::transform(mat4 matrix) {};

Material Object::getMaterial()
{
	return this->material;
}

vec3 Object::convert(vec4 vec)
{
	return vec3(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w);
}

