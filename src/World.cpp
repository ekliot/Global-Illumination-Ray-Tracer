/**
 * World.cpp
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

World::World( vec3 bg, vec3 amb, float _ir ) : background(bg), ambient(amb), ir(_ir) {}

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

std::vector<Light*> World::get_pruned_lights( vec3 point ) {
    std::vector<Light*> ret_lights;

    for ( size_t i = 0; i < lights.size(); i++ ) {
        if ( can_see_light( point, *lights[i] ) ) {
            ret_lights.push_back( lights[i] );
        }
    }

    return ret_lights;
}

bool World::can_see_light( vec3 point, Light light ) {
    vec3 newDirection = *light.position - point;
    Ray r = Ray( &point, &newDirection );
    float distance = 0;

    // TODO we want to be able to tell if one or more of the objects intersected are transparent
    // however, we also want to be able to tell exactly how much light gets through...
    // perhaps this method ought to return not a variable, but a float b/w 0.0-1.0, to represent the percentage of light that reaches the given point from the given light source
    Object* intersect_obj = get_intersected_obj( &r, &distance );

    // if ( intersect_obj != NULL && intersect_obj->get_material()->get_kd() < 1.0f ) {
    //     // newDirection =;
    //     while ( distance < INT_MAX ) {
    //         /* code */
    //     }
    // }

    return ( intersect_obj == NULL || intersect_obj->get_material()->get_ir() > 0.0f );
}

// determine the object intersected by a given object, and store the distance
// @param Ray* r :: a pointer to the Ray to calculate intersection for
// @param float* distance :: the output variable where to store the distance at which the ray intersects (INT_MAX by default)
// @return Object* :: a pointer to the Object that is intersected, or NULL if none are intersected
Object* World::get_intersected_obj( Ray* r, float* distance ){
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

// return a the colour seen by a given Ray at its point of intersection
vec3 World::get_intersect( Ray *ray , mat4 inverse_transform_mat, int depth, Object* last_intersect ) {

    float distance = 0;
    Object* intersect_obj = this->get_intersected_obj( ray, &distance );

    // std::cout << '\n';
    // std::cout << "given ray" << '\n';
    // r->print();

    // if the ray does not hit anything, return the World's background colour
    if ( intersect_obj == NULL ) {
        // std::cout << "nuffin" << '\n';
        return background;
    }

    // otherwise, the Ray has intersected an object
    // so we need to figure out the colour of intersection

    // 3D point of intersection
    vec3 point = *ray->origin + *ray->direction * distance;
    std::vector<Light*> ret_lights = get_pruned_lights( point );

    // the colour of the intersected object seen by the Ray
    vec3 cur_color = intersect_obj->get_color( ray, distance, ret_lights, &ambient, inverse_transform_mat );

    // if we have not hit maximum recursion depth...
    if( depth < MAX_DEPTH ) {
        float kd = intersect_obj->get_material()->get_kd();
        float kr = intersect_obj->get_material()->get_kr();

        // if the object is reflective...
        if ( kr > 0 ) {
            // std::cout << "reflectin..." << '\n';

            vec3 normal_dir = intersect_obj->get_normal( ray, distance );

            vec3 reflect_dir = glm::reflect( *ray->direction, normal_dir );
            Ray reflect_ray = Ray( &point, &reflect_dir );

            // std::cout << "reflect ray:" << '\n';
            // reflect_ray.print();
            // std::cout << '\n';

            vec3 reflect_color = get_intersect( &reflect_ray, inverse_transform_mat, depth+1 );
            cur_color = cur_color + kr * reflect_color;
        }

        // if the object is transparent...
        if ( kd > 0 ) {
            vec3 refract_color = calc_refraction(
                ray, point, distance,
                inverse_transform_mat,
                intersect_obj, last_intersect,
                depth
            );
            cur_color = cur_color + kd * refract_color;
        }
    }

    return cur_color;
}

vec3 World::calc_refraction( Ray* ray, vec3 point, float dist, mat4 inv_trans_mat, Object* intersect, Object* last_isect, int depth ) {
    // std::cout << "transmittin..." << '\n';

    float last_ir = ir;
    float cur_ir  = last_ir;

    // if we're still inside the same object...
    if( last_isect == intersect ) {
        last_ir = last_isect->get_material()->get_ir();
    }
    // or if we are just entering the object...
    else if ( last_isect == NULL ) {
        cur_ir  = intersect->get_material()->get_ir();
    }
    // otherwise, we've just entered a new object...
    else {
        last_ir = last_isect->get_material()->get_ir();
        cur_ir  = intersect->get_material()->get_ir();
    }

    // the normal of the intersected object where the ray intersects it
    vec3 normal = intersect->get_normal( ray, dist );
    // the direction of the ray
    vec3 d = *ray->direction;
    // the new point to be calulating refraction from
    vec3 pt = point + ( *ray->direction * 0.1f );

    float calc_ir = 2.0f - ( cur_ir / last_ir );
    float cosd = dot( normal, d );

    vec3 o = ( d * calc_ir - normal * ( -cosd + calc_ir * cosd ) );

    Ray refract_ray = Ray( &pt, &o );

    // std::cout << "refract ray:" << '\n';
    // refract_ray.print();
    // std::cout << '\n';

    return get_intersect( &refract_ray, inv_trans_mat, depth+1, intersect );
}
