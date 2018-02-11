/**
 * World.cpp
 *
 * Author: ekliot
 */
//#include "stdafx.h"
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <glm\matrix.hpp>
#endif

#include <vector>
#include <glm/vec3.hpp>
using namespace glm;
//#include <glm/mat4.hpp>

#include "Ray.h"
#include "Object.h"
#include "World.h"
#include "Material.h"


World::World(vec4 background) {
	this->background = background;
}

World::~World() {}

std::vector<Object> objects;
vec4 background;

void World::add( Object obj ) {
	objects.push_back(obj);
    //
}

void World::transform_all_to_ccoord( mat4 tmat ) {
	for each (Object object in objects) // Problem part
	{
		object.transform(tmat);
	}
}

vec4 World::get_intersect(Ray r) {

	int value = -1;
	Object* currentObject = NULL;
	for each (Object object in objects)
	{
		int newValue = object.intersection(r);
		if (newValue < value && newValue > 0)
		{
			value = newValue;
			currentObject = &object;
		}
	}
	if (currentObject != NULL)
	{
		return currentObject->getMaterial().getColor();
	}
	return background;
}
