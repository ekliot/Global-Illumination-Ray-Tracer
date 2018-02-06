#include "../../stdafx.h"
#include "stdafx.h"


#ifdef _WIN32
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