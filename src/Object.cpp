#include "stdafx.h"
#ifdef _WIN32
#include <windows.h>
#endif

#include <initializer_list>
#include <vector>
#include "Ray.h"
#include "Material.h"

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

		Material getMaterial()
		{
			return this->mat;
		}

};