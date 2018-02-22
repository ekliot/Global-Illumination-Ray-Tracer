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
#include "Material.h"

World::World( vec4 bg ) : background(bg) {}

World::~World() {}

void World::add_object( Object* obj ) {
    objects.push_back( obj );
}

void World::add_light(Light light){
    lights.push_back( light );
}

void World::transform_all( mat4 tmat ) {
    for ( Object* obj : objects ) {
        obj->transform( tmat );
    }
}

std::vector<Light> World::pruned_lights(vec3 point){
    std::vector<Light> returnLights;
    for (size_t i = 0; i < lights.size(); i++) {
        if(can_see_light(point, lights[i])){
            returnLights.push_back(lights[i]);
        }
    }
    return returnLights;
}
bool World::can_see_light(vec3 point, Light light)
{
    vec3 newDirection = light.position - point;
    Ray r = Ray(&point, &newDirection);
    for ( Object* obj : objects ) {
        int newValue = obj->intersection( &r );
        if ( newValue > 0 ) {
            return true;
        }
    }
    return false;
}

vec4 World::get_intersect( Ray *r ) {
    int value = INT_MAX;
    Object* currentObject = NULL;

    for ( Object* obj : objects ) {
        int newValue = obj->intersection( r );
        if ( newValue < value && newValue > 0 ) {
            value = newValue;
            currentObject = obj;
        }
    }
    if ( currentObject != NULL ) {
        return currentObject->get_material().color;
    }

    return background;
}
