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

		float intersection(Ray ray) { return -1; };
		
		void transform(mat4 matrix) {};

		Material getMaterial()
		{
			return this->mat;
		}

};