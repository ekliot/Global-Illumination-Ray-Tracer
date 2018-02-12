/**
 * World.cpp
 *
 * Author: ekliot
 */
#if defined(_WIN32) || defined(_WIN64)
#include "stdafx.h"
#include <windows.h>
#endif

#include <vector>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

using namespace glm;

#include "World.h"
#include "Material.h"

World::World( vec4 bg ) : background(bg) {}

World::~World() {}

void World::add( Object obj ) {
    objects.push_back(obj);
}

void World::transform_all_to_ccoord( mat4 tmat ) {
    for ( Object obj : objects ) {
        obj.transform( tmat );
    }
}

vec4 World::get_intersect(Ray r) {
    int value = -1;
    Object* currentObject = NULL;
    for ( Object obj : objects ) {
        int newValue = obj.intersection(r);
        if ( newValue < value && newValue > 0 ) {
            value = newValue;
            currentObject = &obj;
        }
    }
    if ( currentObject != NULL ) {
        return currentObject->getMaterial().color;
    }
    return background;
}
