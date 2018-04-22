/**
 * World.cpp
 */

#include <climits>
#include <iostream>
#include <algorithm>
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

std::vector<Light> World::get_pruned_lights( vec3 point ) {
    std::vector<Light> ret_lights;

    for ( size_t i = 0; i < lights.size(); i++ ) {
        Light adj_light = adjusted_light_to_point( point, *lights[i] );
        if ( *(adj_light.color) != vec3( -1.0f ) ) {
            ret_lights.push_back( adj_light );
        }
    }

    return ret_lights;
}

// bool World::can_see_light( vec3 point, Light light ) {
//     vec3 _dir = point - *light.position;
//     // a Ray pointing from the given light to the given point
//     Ray r = Ray( &point, &_dir );
//
//     std::vector<Object*> isecting_objs = get_intersecting_objs( &r );
//
//     bool visible = true;
//
//     for ( Object* obj : isecting_objs ) {
//         visible = visible && (obj->get_material()->get_kd() > 0.0f);
//         if ( !visible ) { break; }
//     }
//
//     return visible;
// }

// returns a light with color RGBs of -1.0f if the given Light is not visible from the given point
Light World::adjusted_light_to_point( vec3 point, Light light ) {
    // vec3 _dir = point - *light.position;
    vec3 _dir = *light.position - point;
    // a Ray pointing from the given point to the given light
    Ray r = Ray( &point, &_dir );

    // a list of objects between the point and Light, sorted by closest->farthest from the Light
    std::vector<Object*> isecting_objs = get_intersecting_objs( &r );

    // deep copy the original light
    vec3* adj_col = new vec3( light.color->x, light.color->y, light.color->z );
    vec3* adj_pos = new vec3( light.position->x, light.position->y, light.position->z );

    bool visible = true;
    float kd = 0.0f;
    vec3 _adj = vec3( 0.0f );

    // for every object between the light and point, modify the colour of the light
    // reaching that point based on the object colours and transparencies
    for ( Object* obj : isecting_objs ) {
        kd = obj->get_material()->get_kd();

        visible = visible && ( kd > 0.0f );
        if ( !visible ) {
            adj_col = new vec3( -1.0f );
            break;
        }
        else {

            // std::cout << "adjusting light..." << '\n';
            //
            // std::cout << "  cur_col // " << glm::to_string(*adj_col) << '\n';
            // std::cout << "  obj_col // " << glm::to_string(obj->get_material()->get_color( 0.0f, 0.0f )) << '\n';
            // std::cout << "  kd // " << kd << '\n';

            // TODO it'd be cool to make this alter the light's colour based on the objet colour...
            // this has failed in the past, however
            // perhaps, if we model light absorption?
            // IDEA light_col - ( vec(1.0f) - obj_col ) * ( 1.0f - kd )
            // _adj = normalize( *adj_col + obj->get_material()->get_color( 0.0f, 0.0f ) ) * kd;

            _adj = *adj_col * ( kd );
            adj_col->x = _adj.x;
            adj_col->y = _adj.y;
            adj_col->z = _adj.z;

            // if ( *adj_col != vec3( -1.0f ) && *adj_col != *light.color ) {
            //     std::cout << "got color: " << glm::to_string( *adj_col ) << '\n';
            //     std::cout << "from:      " << glm::to_string( *light.color ) << '\n';
            // }
        }
    }

    Light ret = {
        adj_col,
        adj_pos
    };

    return ret;
}

std::vector<Object*> World::get_intersecting_objs( Ray* r ) {
    typedef struct st_ObjDist {
        Object* obj;
        float dist;
        // sorts ObjDists in ascending distance from ray origin
        static bool compare( const st_ObjDist &a, const st_ObjDist &b ) {
            return a.dist > b.dist;
        }
    } ObjDist;

    // first collect all intersecting objects with their distance from the ray origin

    std::vector<ObjDist> ods;
    float _dist = 0.0f;

    for ( Object* obj : objects ) {
        _dist = obj->intersection( r );

        if ( _dist < INT_MAX && _dist > 0.00001 ) {
            ods.push_back( { obj, _dist } );
        }
    }

    // then, sort them in ascending distance from the ray origin

    std::sort( ods.begin(), ods.end(), ObjDist::compare );

    std::vector<Object*> objs;

    for ( ObjDist od : ods ) {
        objs.push_back( od.obj );
    }

    return objs;
}

// determine the object intersected by a given object, and store the distance
// @param Ray* r :: a pointer to the Ray to calculate intersection for
// @param float* distance :: the output variable where to store the distance at which the ray intersects (INT_MAX by default)
// @return Object* :: a pointer to the Object that is intersected, or NULL if none are intersected
Object* World::get_intersected_obj( Ray* r, float* distance ){
    Object* ret_obj = NULL;
    *distance = INT_MAX;
    float _dist = 0.0f;

    for ( Object* obj : objects ) {
        _dist = obj->intersection( r );

        if ( _dist < *distance && _dist > 0.00001 ) {
            *distance = _dist;
            ret_obj = obj;
        }
    }

    return ret_obj;
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
    std::vector<Light> ret_lights = get_pruned_lights( point );

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
