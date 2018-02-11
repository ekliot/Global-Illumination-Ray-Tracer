
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <glm\vec4.hpp>;
using namespace glm;

class Material
{
	vec4 color;
	public:
		Material(vec4 color);
		Material();
		vec4 getColor();

};

#endif