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
#include "glm/gtx/string_cast.hpp"

using namespace glm;

#include "World.h"
#include "Light.h"

World::World( vec3 bg, vec3 amb ) : background(bg), ambient(amb) {}

World::~World() {}

void World::add_object( Object* obj ) {
    objects.push_back( obj );
}

void World::add_light( Light* light ){
    lights.push_back( light );
}

void World::transform_all( mat4 tmat ) {
    for ( Object* obj : objects ) {
        obj->transform( tmat );
    }
}

std::vector<Light*> World::pruned_lights(vec3 point){
    std::vector<Light*> returnLights;
    for (size_t i = 0; i < lights.size(); i++) {
        if(can_see_light(point, *lights[i])){
            returnLights.push_back(lights[i]);
        }
    }
    return returnLights;
}
bool World::can_see_light(vec3 point, Light light) {
    vec3 newDirection = *light.position - point;
    Ray r = Ray( &point, &newDirection );
    vec3 isect = get_intersect( &r );
    vec3 comp = glm::equal( isect, background );
    return comp.x && comp.y && comp.z;
}


vec3 World::get_intersect( Ray *r ) {
    float value = INT_MAX;
    Object* currentObject = NULL;

    for ( Object* obj : objects ) {
        float newValue = obj->intersection( r );
        if ( newValue < value && newValue > 0.000001 ) {
            value = newValue;
            currentObject = obj;
        }
    }
    if ( currentObject != NULL ) {
        // do work to do things
        vec3 point = *r->origin + *r->direction * value;
        std::vector<Light*> returnLights = pruned_lights(point);
        return currentObject->get_color( r, value, returnLights, &ambient );
    }

    return background;
}
