#ifdef _WIN32
#include "stdafx.h"
#include <windows.h>
#endif

#include <initializer_list>
#include <vector>
#include "Ray.h"


class Object
{
	public :
		virtual float intersection(Ray ray) = 0;
};