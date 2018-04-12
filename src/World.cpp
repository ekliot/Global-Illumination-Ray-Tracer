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
vec3 World::get_intersect( Ray *r , mat4 inverse_transform_mat, int depth, Object* lastIntersectionObject ) {

    float distance = 0;
    Object* intersect_obj = this->get_intersect_helper( r, &distance );

    // TODO SPLIT THIS SHIT

    if ( intersect_obj != NULL ) {
        // do work to do things
        vec3 point = *r->origin + *r->direction * distance;

        float kd = intersect_obj->get_material()->get_kd();
        std::vector<Light*> ret_lights = pruned_lights( point );
        if (kd > 0){
            ret_lights = lights;
        }

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
            if (kd > 0){
                vec3 newPoint = point + (*r->direction * .1f );

                float lastIR = 1.0;
                float currentIR = 1.0;
                if(lastIntersectionObject == intersect_obj)
                {
                    lastIR = lastIntersectionObject->get_material()->get_ir();
                    currentIR = 1.0;
                }
                else if (lastIntersectionObject == NULL){
                    lastIR = 1.0;
                    currentIR = intersect_obj->get_material()->get_ir();
                }
                else{
                    lastIR = lastIntersectionObject->get_material()->get_ir();
                    currentIR = intersect_obj->get_material()->get_ir();
                }




                vec3 normal_dir = intersect_obj->get_normal( r, distance );
                float calcIR = 2.0f - (currentIR/lastIR);
                vec3 i = *r->direction;
                float cosi = dot(normal_dir, i);
                vec3 o = (i * calcIR - normal_dir * (-cosi + calcIR * cosi));





                //vec3 refraction_dir = refract(*r->direction, normal_dir, 1.0f);
                Ray ref_ray = Ray( &newPoint, &o);





                // std::cout << '\n';
                // std::cout << "given ray" << '\n';
                // r->print();
                // std::cout << "calculated ray" << '\n';
                // ref_ray.print();
                // std::cout << '\n';

                vec3 ref_color = get_intersect( &ref_ray, inverse_transform_mat, depth + 1, intersect_obj);
                cur_color = cur_color+ kd * ref_color;


            }
            else{

            }
        }

        return cur_color;
    }

    return background;
}
