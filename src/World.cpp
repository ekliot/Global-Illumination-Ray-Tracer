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
    std::vector<Light*> ret_lights;
    for ( size_t i = 0; i < lights.size(); i++ ) {
        if( can_see_light( point, *lights[i]) ){
            ret_lights.push_back( lights[i] );
        }
    }
    return ret_lights;
}

bool World::can_see_light( vec3 point, Light light ) {
    vec3 newDirection = *light.position - point;
    Ray* r = new Ray( &point, &newDirection );
    float distance = 0;
    Object* intersect_obj = get_intersect_helper( r, &distance );
    delete r;
    return ( intersect_obj == NULL );

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

// I think this is where we should do the thing
vec3 World::get_intersect( Ray *r , mat4 inverse_transform_mat, int depth ) {

    float distance = 0;
    Object* intersect_obj = this->get_intersect_helper( r, &distance );

    // TODO SPLIT THIS SHIT

    if ( intersect_obj != NULL ) {
        // do work to do things
        vec3 point = *r->origin + *r->direction * distance;
        std::vector<Light*> ret_lights = pruned_lights( point );

        vec3 cur_color = intersect_obj->get_color( r, distance, ret_lights, &ambient, inverse_transform_mat );

        if( depth < MAX_DEPTH ) {
            float kr = intersect_obj->get_material()->get_kr();
            if( kr > 0 ) {
                vec3 normal_dir = intersect_obj->get_normal( r, distance );

                vec3 ref_dir = glm::reflect( *r->direction, normal_dir );
                Ray ref_ray = Ray( &point, &ref_dir );

                vec3 ref_color = get_intersect( &ref_ray, inverse_transform_mat, depth + 1 );
                cur_color = cur_color+ kr * ref_color;
            }
        }

        return cur_color;
    }

    return background;
}
