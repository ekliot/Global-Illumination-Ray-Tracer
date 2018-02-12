//#include "stdafx.h"
#ifdef _WIN32
#include "../../stdafx.h"
#include <windows.h>
#endif

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "Object.h"
#include "Material.h"
#include "Triangle.h"

using namespace glm;

	

Triangle::Triangle(vec3 a, vec3 b, vec3 c, Material mat): Object(mat){
	this->a = a;
	this->b = b;
	this->c = c;
}

// Compute barycentric coordinates (u, v, w) for
// point p with respect to triangle (a, b, c)
float Triangle::getBarycentricCoords(vec3 p)
{
	vec3 v0 = b - a; vec3 v1 = c - a; vec3 v2 = p - a;

	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);

	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;
	return u;
}

float Triangle::intersectPlane(Ray ray)
{
	vec3 ab = b - a;
	vec3 ac = c - a;
	// no need to normalize
	vec3 n =  cross(ab,ac); // N 
	vec3 p0 = a;

	// assuming vectors are all normalized
	float denom = dot(n, ray.direction);
	if (denom > 1e-6) {
		vec3 p0l0 = p0 - ray.point;
		float t = dot(p0l0, n) / denom;
		if (t >= 0)
		{
			return t;
		}
	}
	return -1;
}

float Triangle::intersection(Ray ray) {

	float mag = intersectPlane(ray);
	if (mag != -1)
	{
		vec3 point = ray.direction * mag;
		float u = getBarycentricCoords(point);
		if (u >= 0 && u <= 1)
		{
			return mag;
		}
	}
	return -1;
}

void Triangle::transform(mat4 matrix)
{
	vec4 a4 = vec4(this->a.x, this->a.y, this->a.z, 1);
	a4 = matrix * a4;
	a = convert(a4);

	vec4 b4 = vec4(this->b.x, this->b.y, this->b.z, 1);
	b4 = matrix * b4;
	b = convert(b4);

	vec4 c4 = vec4(this->c.x, this->c.y, this->c.z, 1);
	c4 = matrix * c4;
	c = convert(c4);
}
