//#include "stdafx.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <glm\vec4.hpp>;
using namespace glm;

#include "Material.h";

	

Material::Material(vec4 color)
{
	this->color = color;
}

Material::Material()
{
	this->color = vec4(255,255,255,255);
}

vec4 Material::getColor()
{
	return this->color;
}

