/**
 * World.cpp
 *
 * Author: ekliot
 */

#include <climits>
#include <iostream>
#include <vector>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

using namespace glm;

#include "World.h"
<<<<<<< HEAD
#include "Material.h"
#include "Light.h"
=======
>>>>>>> chkpt-3

World::World( vec4 bg ) : background(bg) {}

World::~World() {}

void World::add( Object* obj ) {
    objects.push_back( obj );
}

void World::transform_all( mat4 tmat ) {
    for ( Object* obj : objects ) {
        obj->transform( tmat );
    }
}

vec4 World::get_intersect( Ray *r ) {
    float value = INT_MAX;
    Object* currentObject = NULL;

    for ( Object* obj : objects ) {
        float newValue = obj->intersection( r );
        if ( newValue < value && newValue > 0 ) {
            value = newValue;
            currentObject = obj;
        }
    }
    if ( currentObject != NULL ) {
<<<<<<< HEAD
      // do work to do things
      std::vector<Light> lights;
      return currentObject->get_color(*r, value, lights);
=======
        // TODO this should return whatever the object's IlluminationModel returns for intersection
        // return currentObject->get_material().color;
        return background;
>>>>>>> chkpt-3
    }

    return background;
}
