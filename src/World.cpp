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

World::~World() {
    for ( Object* o : objects ) {
        delete o;
    }

    for ( Light* l : lights ) {
        delete l->color;
        delete l->position;
    }
}

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

std::vector<Light*> World::pruned_lights( vec3 point ) {
    std::vector<Light*> returnLights;
    for ( size_t i = 0; i < lights.size(); i++ ) {
        if( can_see_light( point, *lights[i]) ){
            returnLights.push_back( lights[i] );
        }
    }
    return returnLights;
}
bool World::can_see_light( vec3 point, Light light ) {
    vec3 newDirection = *light.position - point;
    Ray* r = new Ray( &point, &newDirection );
    float distance = 0;
    Object* intersectObject = get_intersect_helper( r, &distance );
    delete r;
    return ( intersectObject == NULL );

}

Object* World::get_intersect_helper( Ray * r, float* distance ){
  Object* returnObject = NULL;
  *distance = INT_MAX;
  for ( Object* obj : objects ) {
      float newValue = obj->intersection( r );
      if ( newValue < *distance && newValue > 0.00001 ) {
          *distance = newValue;
          returnObject = obj;
      }
  }

  return returnObject;
}

vec3 World::get_intersect( Ray *r , mat4 inverse_transform_mat ) {

    float distance = 0;
    Object* intersectObject = this->get_intersect_helper( r, &distance );

    // TODO SPLIT THIS SHIT

    if ( intersectObject != NULL ) {
        // do work to do things
        vec3 point = *r->origin + *r->direction * distance;
        std::vector<Light*> returnLights = pruned_lights( point );
        return intersectObject->get_color( r, distance, returnLights, &ambient, inverse_transform_mat );
    }

    return background;
}
