/**
 * World.cpp
 *
 * Author: ekliot
 */
#include "stdafx.h"

#if defined(_WIN32)
#include <windows.h>
#endif

#include <climits>
#include <iostream>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

using namespace glm;

#include "World.h"
#include "Material.h"

World::World( vec4 bg ) : background(bg) {}

World::~World() {}

void World::add( Object* obj ) {
    objects.push_back( obj );
}

void World::transform_all_to_ccoord( mat4 tmat ) {
    for ( Object* obj : objects ) {
        obj->transform( tmat );
    }
}

vec4 World::get_intersect( Ray *r ) {
    int value = INT_MAX;
    Object* currentObject = NULL;

    for ( Object* obj : objects ) {
        int newValue = obj->intersection( r );
        if ( newValue < value && newValue > 0 ) {
            // std::cout << "BARB" << newValue << '\n';
            value = newValue;
            currentObject = obj;
        }
    }
    if ( currentObject != NULL ) {
        // std::cout << "BOOYAH" << '\n';
        return currentObject->getMaterial().color;
    }

    return background;
}
