#include "stdafx.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include "Ray.h"
#include "Material.h"
#include <glm\matrix.hpp>

using namespace glm;

class Object
{
	private:
		Material mat;

	public :

		Object(Material material)
		{
			this->mat = material;
		}

		virtual float intersection(Ray ray) = 0;

		virtual void transform(mat4 matrix) = 0;

		Material getMaterial()
		{
			return this->mat;
		}

};