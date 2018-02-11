//#include "stdafx.h"
#include "Material.h"

#ifdef _WIN32
#include <windows.h>
#endif

class Material
{
	vec4 color;
	public:

		Material(vec4 color)
		{
			this->color = color;
		}

		vec4 getColor()
		{
			return this->color;
		}

};