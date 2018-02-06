#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Ray.h";
#include "Material.h";
#include <glm\matrix.hpp>;
using namespace glm;

class Object
{
	private:
		Material material;

	public :
		Object(Material mat);
		virtual float intersection(Ray ray);
		virtual void transform(mat4 matrix);
		Material getMaterial();


};
#endif