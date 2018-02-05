
#ifdef _WIN32
#include "stdafx.h"
#include <windows.h>
#endif

class Material
{
	int* color;

	public:

		Material(int* color)
		{
			this->color = color;
		}

		int* getColor()
		{
			return this->color;
		}

};