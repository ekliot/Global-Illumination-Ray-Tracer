/**
 * World.cpp
 */

#include "World.h"

#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <glm/vec4.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "Phong.h"
#include "SolidMaterial.h"
#include "SquareLight.h"
#include "glm/gtx/string_cast.hpp"
#include "tinyply.h"

using namespace tinyply;
using namespace photon;

World::World( vec3 bg, vec3 amb, float _ir )
    : background( bg ), ambient( amb ), ir( _ir ) {}

World::~World() {
    for ( Object* o : objects ) {
        if ( o != nullptr ) {
            delete o;
            o = nullptr;
        }
    }

    for ( Light* l : lights ) {
        if ( l != nullptr ) {
            delete l;
            l = nullptr;
        }
    }

    // delete global_pmap;
    // delete caustic_pmap;
    // delete shadow_pmap;
    // // delete volume_pmap;
    //
    // for ( Photon* p : global_photons ) {
    //     delete p;
    // }
    //
    // for ( Photon* p : caustic_photons ) {
    //     delete p;
    // }
    //
    // for ( Photon* p : shadow_photons ) {
    //     delete p;
    // }
    //
    // for ( Photon* p : volume_photons ) {
    //     delete p;
    // }
}

void World::add_object( Object* obj ) { objects.push_back( obj ); }

void World::add_light( Light* light ) {
    lights.push_back( light );
    if ( SquareLight* sq_light = dynamic_cast<SquareLight*>( light ) ) {
        objects.push_back( sq_light->get_obj() );
        // std::cout << "pushed stuff";
    }
}

void World::transform_all( mat4 tmat ) {
    for ( Object* obj : objects ) {
        obj->transform( tmat );
    }
    for ( Light* l : lights ) {
        l->transform( tmat );

        // we have to do this after the transform to it doesnt get double
        // transformed
    }
}

std::vector<Light*> World::get_pruned_lights( vec3 point ) {
    std::vector<Light*> ret_lights;

    for ( Light* l : lights ) {
        // Light* adj_light = adjusted_light_to_point( point, l );
        // if ( adj_light->get_color() != vec3( -1.0f ) ) {
        if ( can_see_light( point, l ) ) {
            ret_lights.push_back( l );
        }
    }

    return ret_lights;
}

bool World::can_see_light( vec3 point, Light* light ) {
    // FIXME this only checks the center of the light, not the area of a light
    vec3 _dir = light->get_pos() - point;

    float dist = length( _dir );
    // a Ray pointing from the given light to the given point
    Ray r = Ray( &point, &_dir );

    std::vector<Object*> isecting_objs = get_intersecting_objs( &r, dist );

    bool visible = true;
    bool is_light;
    // bool is_transparent;

    for ( Object* obj : isecting_objs ) {
        if ( SquareLight* sq_light = dynamic_cast<SquareLight*>( light ) ) {
            is_light = sq_light->get_obj() == obj;
            if ( is_light ) {
                // std::cout << "sq_lite >> " << '\n';
            }
        } else {
            is_light = false;
        }

        // is_transparent = obj->get_material()->get_kd() > 0.0f;

        visible = visible && is_light;
        if ( !visible ) {
            break;
        }
    }

    return visible;
}

// COMBAK don't need this?
// returns a light with color RGBs of -1.0f if the given Light is not visible
// from the given point
// Light* World::adjusted_light_to_point( vec3 point, Light* light ) {
//     // vec3 _dir = point - *light.position;
//     vec3 _dir = light->get_pos() - point;
//
//     float dist = length( _dir );
//     // a Ray pointing from the given point to the given light
//     Ray r = Ray( &point, &_dir );
//
//     // a list of objects between the point and Light, sorted by
//     // closest->farthest from the Light
//     std::vector<Object*> isecting_objs = get_intersecting_objs( &r, dist );
//
//     // deep copy the original light
//     vec3* adj_col = new vec3( light->get_color() );
//     vec3* adj_pos = new vec3( light->get_pos() );
//
//     bool visible = true;
//     float kd     = 0.0f;
//     vec3 _adj    = vec3( 0.0f );
//
//     // for every object between the light and point, modify the colour of the
//     // light reaching that point based on the object colours and
//     transparencies for ( Object* obj : isecting_objs ) {
//         kd = obj->get_material()->get_kd();
//
//         visible = visible && ( kd > 0.0f );
//         if ( !visible ) {
//             adj_col = new vec3( -1.0f );
//             break;
//         } else {
//             // std::cout << "adjusting light..." << '\n';
//             //
//             // std::cout << "  cur_col // " << glm::to_string(*adj_col) <<
//             '\n';
//             // std::cout << "  obj_col // " <<
//             // glm::to_string(obj->get_material()->get_color( 0.0f, 0.0f ))
//             <<
//             // '\n'; std::cout << "  kd // " << kd << '\n';
//
//             // TODO it'd be cool to make this alter the light's colour based
//             on
//             // the objet colour... this has failed in the past, however
//             perhaps,
//             // if we model light absorption? IDEA light_col - ( vec(1.0f) -
//             // obj_col ) * ( 1.0f - kd ) _adj = normalize( *adj_col +
//             // obj->get_material()->get_color( 0.0f, 0.0f ) ) * kd;
//
//             _adj       = *adj_col * ( kd );
//             adj_col->x = _adj.x;
//             adj_col->y = _adj.y;
//             adj_col->z = _adj.z;
//
//             // if ( *adj_col != vec3( -1.0f ) && *adj_col != *light.color ) {
//             //     std::cout << "got color: " << glm::to_string( *adj_col )
//             <<
//             //     '\n'; std::cout << "from:      " << glm::to_string(
//             //     *light.color ) << '\n';
//             // }
//         }
//     }
//
//     // Light* ret = new Light();
//
//     return light;
// }

std::vector<Object*> World::get_intersecting_objs( Ray* r, float dist ) {
    typedef struct st_ObjDist {
        Object* obj;
        float dist;
        // sorts ObjDists in ascending distance from ray origin
        static bool compare( const st_ObjDist& a, const st_ObjDist& b ) {
            return a.dist > b.dist;
        }
    } ObjDist;

    // first collect all intersecting objects with their distance from the ray
    // origin

    std::vector<ObjDist> ods;
    float _dist = 0.0f;

    for ( Object* obj : objects ) {
        _dist = obj->intersection( r );

        if ( _dist < INT_MAX && _dist > 0.00001 && _dist <= dist ) {
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

// determine the object intersected by a given ray, and store the distance
// @param Ray* r :: a pointer to the Ray to calculate intersection for
// @param float* distance :: the output variable where to store the distance at
// which the ray intersects (INT_MAX by default)
// @return Object* :: a pointer to the Object that is intersected, or NULL if
// none are intersected
Object* World::get_intersected_obj( Ray* r, float* distance ) {
    Object* ret_obj = NULL;
    *distance       = INT_MAX;
    float _dist     = 0.0f;

    for ( Object* obj : objects ) {
        _dist = obj->intersection( r );

        if ( _dist < *distance && _dist > 0.00001 ) {
            *distance = _dist;
            ret_obj   = obj;
        }
    }

    // for ( Light* l : lights ) {
    //     _dist = l->intersection( r );
    //
    //     if ( _dist < *distance && _dist > 0.00001 ) {
    //         *distance = _dist;
    //         // FIXME should be Light::get_obj() without casting, check
    //         Light.h
    //         // for why this doesn't work
    //         if ( SquareLight* sq_light = dynamic_cast<SquareLight*>( l ) ) {
    //             ret_obj = sq_light->get_obj();
    //         }
    //     }
    // }

    return ret_obj;
}

// return a the colour seen by a given Ray at its point of intersection
vec3 World::get_intersect( Ray* ray, int depth, Object* last_intersect ) {
    float distance = 0;

    // Object* obj = this->get_intersect_kd_tree( ray, &distance );
    Object* obj = this->get_intersected_obj( ray, &distance );

    // std::cout << '\n';
    // std::cout << "given ray" << '\n';
    // r->print();

    // if the ray does not hit anything, return the World's background colour
    if ( obj == NULL ) {
        // std::cout << "nuffin" << '\n';
        return background;
    }

    // otherwise, the Ray has intersected an object
    // so we need to figure out the colour of intersection

    // 3D point of intersection

    vec3 position  = *ray->origin + *ray->direction * distance;
    vec3 cur_color = radiance( position, ray, distance, obj, RAD_EST,
                               last_intersect, depth );

    return cur_color;
}

vec3 World::calc_refraction( Ray* ray, vec3 point, float dist,
                             Object* intersect, Object* last_isect,
                             int depth ) {
    // std::cout << "transmittin..." << '\n';

    float last_ir = ir;
    float cur_ir  = last_ir;

    // if we're still inside the same object...
    if ( last_isect == intersect ) {
        last_ir = last_isect->get_material()->get_ir();
    }
    // or if we are just entering the object...
    else if ( last_isect == NULL ) {
        cur_ir = intersect->get_material()->get_ir();
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
    float cosd    = dot( normal, d );

    vec3 o = ( d * calc_ir - normal * ( -cosd + calc_ir * cosd ) );

    Ray refract_ray = Ray( &pt, &o );

    // std::cout << "refract ray:" << '\n';
    // refract_ray.print();
    // std::cout << '\n';

    return get_intersect( &refract_ray, depth + 1, intersect );
}

vec3 World::calc_reflection( Ray* ray, vec3 point, float dist,
                             Object* intersect, int depth ) {
    // std::cout << "reflectin..." << '\n';

    vec3 normal_dir = intersect->get_normal( ray, dist );

    vec3 reflect_dir = glm::reflect( *ray->direction, normal_dir );
    vec3 newPoint    = point + .1f * reflect_dir;

    Ray reflect_ray = Ray( &newPoint, &reflect_dir );

    // std::cout << "reflect ray:" << '\n';
    // reflect_ray.print();
    // std::cout << '\n';

    return get_intersect( &reflect_ray, depth + 1, intersect );
}

/**********\
  K-D TREE
\**********/

void World::generate_kd_tree() {
    AABB* currentAABB =
        new AABB( -FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX );
    for ( Object* obj : objects ) {
        currentAABB = new AABB( currentAABB, obj->get_aabb() );
    }
    obj_tree = new KDTreeNode( objects, currentAABB, 0 );
}

Object* World::get_intersect_kd_tree_helper( Ray* r, KDTreeNode* node,
                                             float* returnDist ) {
    *returnDist       = FLT_MAX;
    Object* returnObj = NULL;

    if ( node->left == NULL ) {
        *returnDist = INT_MAX;

        for ( Object* obj : node->objects ) {
            float newValue = obj->intersection( r );
            if ( newValue < *returnDist && newValue > 0.00001 ) {
                *returnDist = newValue;
                returnObj   = obj;
            }
        }
        if ( returnObj != NULL ) return returnObj;
    }
    if ( node->left == NULL ) {
        return NULL;
    }

    float a_enter = node->left->aabb->intersect_ray( r );
    float b_enter = node->right->aabb->intersect_ray( r );

    if ( a_enter < 0 ) a_enter = INT_MAX;

    if ( b_enter < 0 ) b_enter = INT_MAX;

    if ( a_enter == INT_MAX && b_enter == INT_MAX ) {
        return NULL;
    }

    if ( abs( a_enter - b_enter ) < 0.00001 ) {
        float a_dist;
        Object* a_vec = get_intersect_kd_tree_helper( r, node->left, &a_dist );

        float b_dist;

        Object* b_vec = get_intersect_kd_tree_helper( r, node->right, &b_dist );
        if ( a_dist < b_dist ) {
            *returnDist = a_dist;
            return a_vec;
        } else {
            *returnDist = b_dist;
            return b_vec;
        }

    } else if ( a_enter <= b_enter && a_enter != INT_MAX ) {
        Object* a_vec =
            get_intersect_kd_tree_helper( r, node->left, returnDist );
        if ( a_vec != NULL ) {
            return a_vec;
        }
        Object* b_vec =
            get_intersect_kd_tree_helper( r, node->right, returnDist );
        if ( b_enter != INT_MAX ) {
            return b_vec;
        }
    } else if ( b_enter < a_enter && b_enter != INT_MAX ) {
        Object* b_vec =
            get_intersect_kd_tree_helper( r, node->right, returnDist );
        if ( b_vec != NULL ) {
            return b_vec;
        }
        Object* a_vec =
            get_intersect_kd_tree_helper( r, node->left, returnDist );
        if ( a_enter != INT_MAX ) {
            return a_vec;
        }
    }

    *returnDist = FLT_MAX;
    return NULL;
}

Object* World::get_intersect_kd_tree( Ray* r, float* returnDist ) {
    Object* obj = get_intersect_kd_tree_helper( r, obj_tree, returnDist );
    return obj;
}

void World::add_bunny() {
    try {
        const std::string filename = "src/tinyply/bunny.ply";
        // Read the file and create a std::istringstream suitable
        // for the lib -- tinyply does not perform any file i/o.
        std::ifstream ss( filename, std::ios::binary );

        if ( ss.fail() ) {
            throw std::runtime_error( "failed to open " );
        }

        PlyFile file;
        std::shared_ptr<PlyData> vertices, faces;

        file.parse_header( ss );

        try {
            vertices = file.request_properties_from_element(
                "vertex", { "x", "y", "z" } );
        } catch ( const std::exception& e ) {
            std::cerr << "tinyply exception: " << e.what() << std::endl;
        }

        try {
            faces = file.request_properties_from_element(
                "face", { "vertex_indices" } );
        } catch ( const std::exception& e ) {
            std::cerr << "tinyply exception: " << e.what() << std::endl;
        }

        file.read( ss );

        const size_t numVerticesBytes = vertices->buffer.size_bytes();

        struct float3 {
            float x, y, z;
        };
        std::vector<float3> verts( vertices->count );
        std::memcpy( verts.data(), vertices->buffer.get(), numVerticesBytes );

        const size_t numFacesBytes = faces->buffer.size_bytes();
        struct int3 {
            uint x, y, z;
        };
        std::vector<int3> triangles( faces->count );
        std::memcpy( triangles.data(), faces->buffer.get(), numFacesBytes );

        std::vector<Triangle*> tris;

        vec3 translate = vec3( 0.77f, 2.7f, -5.0f );

        for ( size_t count = 0; triangles.size() > count; count++ ) {
            float3 firstVert = verts[triangles[count].x];
            vec3 firstVec =
                vec3( firstVert.x, firstVert.y, firstVert.z ) + translate;

            float3 secondVert = verts[triangles[count].y];
            vec3 secondVec =
                vec3( secondVert.x, secondVert.y, secondVert.z ) + translate;

            float3 thirdVert = verts[triangles[count].z];
            vec3 thirdVec =
                vec3( thirdVert.x, thirdVert.y, thirdVert.z ) + translate;

            Phong* bunny_imodel = new Phong( vec3( 1.0f, 1.0f, 1.0f ),
                                             // ka,  kd,   ks,   ke
                                             0.1f, 0.5f, 0.1f, 20.0f );
            SolidMaterial* bunny_mat =
                new SolidMaterial( vec3( 0.8f, 0.2f, 0.2f ), 0.0f, 0.0f );

            // add triangle here
            Triangle* triangle = new Triangle( firstVec, secondVec, thirdVec,
                                               bunny_imodel, bunny_mat );
            add_object( triangle );
        }
    } catch ( const std::exception& e ) {
        std::cerr << "Caught tinyply exception: " << e.what() << std::endl;
    }
}

/*****************\
  PHOTON RADIANCE
\*****************/

void World::emit_photons( size_t photon_count ) {
    vector<Photon*> _photons;
    vector<Photon*> photons = vector<Photon*>();
    size_t p_per_light;

    for ( Light* l : lights ) {
        p_per_light = static_cast<size_t>( photon_count / lights.size() );

        while ( p_per_light > 0 ) {
            _photons = l->emit_photons( p_per_light );
            /* remove any photon being wasted */
            // _photons = trim_photons( _photons );
            p_per_light -= _photons.size();
        }

        photons.insert( photons.end(), _photons.begin(), _photons.end() );
    }

    for ( Photon* p : photons ) {
        trace_photon( p, false, false );
        // std::cout << "Photon:" << '\n';
        // std::cout << "\tpos // " << glm::to_string( p->position ) << '\n';
        // std::cout << "\tpow // " << glm::to_string( p->power ) << '\n';
        // std::cout << "\tdir // " << glm::to_string( p->dir ) << '\n';
        // std::cout << '\n';
    }

    build_photon_maps();
}

vector<Photon*> World::trim_photons( vector<Photon*> photons ) {
    vector<Photon*> trimmed = vector<Photon*>();
    float distance          = 0.0f;
    Ray* r;
    Object* intersect_obj;

    for ( Photon* p : photons ) {
        r             = new Ray( new vec3( p->position ), new vec3( p->dir ) );
        intersect_obj = get_intersect_kd_tree( r, &distance );

        if ( intersect_obj != NULL ) {
            trimmed.push_back( p );
        } else {
            delete p;
        }

        delete r;
    }

    return trimmed;
}

Photon* World::trace_volumetric( Photon* p, float current_distance,
                                 float max_distance ) {
    float scatering_albedo = 0.1f;
    float absorb_albedo    = 0.1f;
    float step             = 0.1f;

    float distance;
    if ( current_distance == 0 ) {
        Ray r = Ray( new vec3( p->position ), new vec3( p->dir ) );
        Object* intersect_obj = this->get_intersected_obj( &r, &distance );
        if ( intersect_obj != NULL ) {
            max_distance = distance;
        }
    }

    // exit condition
    if ( current_distance > max_distance ) {
        return p;
    }

    float random =
        static_cast<float>( rand() ) / static_cast<float>( RAND_MAX );

    if ( random < scatering_albedo ) {
        //scatter
        Photon* next_p   = new Photon();
        next_p->power    = vec3( p->power );
        next_p->position = vec3( p->position + p->dir * current_distance );
        next_p->dir      = vec3( p->dir );
        p->is_shadow     = false;
        volume_photons.push_back( p );

    } else if ( random < scatering_albedo + absorb_albedo ) {
        //delete this
        return NULL;
    }

    return trace_volumetric( p, current_distance + step, max_distance );
}

void World::trace_photon( Photon* p, bool was_specular, bool diffused ) {
    Ray* r = new Ray( new vec3( p->position ), new vec3( p->dir ) );

    float distance        = 0;
    Object* intersect_obj = get_intersect_kd_tree( r, &distance );

    if ( intersect_obj != NULL ) {
        vec3 obj_norm = intersect_obj->get_normal( r, distance );
        p->position   = vec3( p->position + p->dir * distance );

        if ( !was_specular && !diffused ) {
            shadow_photons.push_back( p );

            // generate shadow photons
            trace_shadow( p );
        }

        float random =
            static_cast<float>( rand() ) / static_cast<float>( RAND_MAX );

        float kd = intersect_obj->get_imodel()->get_kd();
        float ks = intersect_obj->get_imodel()->get_ks();

        // RUSSIAN ROULETTE

        if ( random < kd ) {
            if ( was_specular ) {
                caustic_photons.push_back( p );
            }

            if ( diffused ) {
                global_photons.push_back( p );
            }

            Ray* diff_dir = r->reflect( &obj_norm );

            Photon* diff_p   = new Photon();
            diff_p->power    = vec3( p->power );
            diff_p->src      = vec3( p->src );
            diff_p->position = vec3( p->position );
            diff_p->dir      = vec3( *diff_dir->direction );

            delete diff_dir;

            trace_photon( diff_p, was_specular, true );
        } else if ( random < kd + ks ) {
            // specular reflection

            float k_ref   = intersect_obj->get_material()->get_kr();
            float k_trans = intersect_obj->get_material()->get_kd();

            if ( k_ref > 0.0f ) {
                Ray* ref_dir = r->reflect( &obj_norm );

                Photon* reflect_p = new Photon();
                reflect_p->power  = vec3( p->power );
                reflect_p->src    = vec3( p->src );
                reflect_p->position =
                    vec3( p->position + *ref_dir->direction * 0.1f );
                reflect_p->dir = vec3( *ref_dir->direction );

                delete ref_dir;

                trace_photon( reflect_p, true, diffused );
            }

            if ( k_trans > 0.0f ) {
                // first, step through the object
                float last_ir = ir;
                float cur_ir  = intersect_obj->get_material()->get_ir();

                // the direction of the ray
                vec3 cur_dir = p->dir;
                // the new point to be calulating refraction from
                vec3 check = p->position + ( cur_dir * 0.1f );

                // what direction will the ray go once inside?
                float calc_ir = 2.0f - ( cur_ir / last_ir );
                float cosd    = dot( obj_norm, cur_dir );
                vec3 ref_dir  = ( cur_dir * calc_ir -
                                 obj_norm * ( -cosd + calc_ir * cosd ) );

                // shoot a ray inside the object and (hopefully) get the opposite side
                Ray* refract_ray   = new Ray( &check, &ref_dir );
                float refract_dist = 0.0f;
                Object* next_obj =
                    get_intersect_kd_tree( refract_ray, &refract_dist );

                // then, when we get to the opposite side, shoot the photon
                if ( next_obj == intersect_obj && refract_dist > 0.0f ) {
                    vec3 pos =
                        *refract_ray->origin +
                        *refract_ray->direction * ( refract_dist + 0.01f );

                    last_ir = cur_ir;  // the object IR
                    cur_ir  = ir;      // the world IR

                    vec3 next_norm =
                        next_obj->get_normal( refract_ray, refract_dist );

                    calc_ir = 2.0f - ( cur_ir / last_ir );
                    cosd    = dot( next_norm, *refract_ray->direction );
                    ref_dir = ( *refract_ray->direction * calc_ir -
                                next_norm * ( -cosd + calc_ir * cosd ) );

                    Photon* refract_p   = new Photon();
                    refract_p->position = pos;
                    refract_p->power    = vec3( p->power );
                    refract_p->dir      = vec3( normalize( ref_dir ) );
                    refract_p->src      = vec3( p->src );

                    delete refract_ray;

                    trace_photon( refract_p, true, diffused );
                } else {
                    // std::cout << "  !!! refraction messed up dood !!!" << '\n';
                    // std::cout << "something went wrong with refraction :c "
                    //           << refract_dist << '\n';
                }
            }
        } else {
            // absorption
        }
    }

    delete r;
}

void World::trace_shadow( Photon* p ) {
    vec3 step_pos   = p->position + p->dir * 0.1f;
    float step_dist = 0.0f;
    Ray* shadow_ray = new Ray( &step_pos, &p->dir );

    Object* isect = get_intersect_kd_tree( shadow_ray, &step_dist );

    Photon* shadow_p;

    while ( isect != NULL ) {
        // std::cout << "sending shadow..." << '\n';

        step_pos = step_pos + p->dir * step_dist;

        shadow_p            = new Photon();
        shadow_p->position  = vec3( step_pos );
        shadow_p->power     = vec3( 0.0f );
        shadow_p->dir       = vec3( p->dir );
        shadow_p->src       = vec3( p->src );
        shadow_p->is_shadow = true;

        // std::cout << "\tpos   // " << glm::to_string( shadow_p->position )
        //           << '\n';
        // std::cout << "\tdir   // " << glm::to_string( shadow_p->dir ) << '\n';

        shadow_photons.push_back( shadow_p );

        step_pos = step_pos + p->dir * 0.1f;

        shadow_ray->origin = &step_pos;

        isect = get_intersect_kd_tree( shadow_ray, &step_dist );
    }
}

void World::build_photon_maps() {
    std::cout << "global  // " << global_photons.size() << '\n';
    std::cout << "shadow  // " << shadow_photons.size() << '\n';
    std::cout << "caustic // " << caustic_photons.size() << '\n';

    using std::max;
    using std::min;

    vec3 v_max;
    vec3 v_min;

    //

    v_max = vec3( FLT_MIN );
    v_min = vec3( FLT_MAX );

    for ( Photon* p : global_photons ) {
        v_max.x = max( v_max.x, p->position.x );
        v_max.y = max( v_max.y, p->position.y );
        v_max.z = max( v_max.z, p->position.z );

        v_min.x = min( v_min.x, p->position.x );
        v_min.y = min( v_min.y, p->position.y );
        v_min.z = min( v_min.z, p->position.z );
    }

    AABB* global_aabb = new AABB( v_max, v_min );

    //

    v_max = vec3( FLT_MIN );
    v_min = vec3( FLT_MAX );

    for ( Photon* p : shadow_photons ) {
        v_max.x = max( v_max.x, p->position.x );
        v_max.y = max( v_max.y, p->position.y );
        v_max.z = max( v_max.z, p->position.z );

        v_min.x = min( v_min.x, p->position.x );
        v_min.y = min( v_min.y, p->position.y );
        v_min.z = min( v_min.z, p->position.z );
    }

    AABB* shadow_aabb = new AABB( v_max, v_min );

    //

    v_max = vec3( FLT_MIN );
    v_min = vec3( FLT_MAX );

    for ( Photon* p : caustic_photons ) {
        v_max.x = max( v_max.x, p->position.x );
        v_max.y = max( v_max.y, p->position.y );
        v_max.z = max( v_max.z, p->position.z );

        v_min.x = min( v_min.x, p->position.x );
        v_min.y = min( v_min.y, p->position.y );
        v_min.z = min( v_min.z, p->position.z );
    }

    AABB* caustic_aabb = new AABB( v_max, v_min );

    global_pmap  = new PhotonKDTreeNode( global_photons, global_aabb, 0 );
    shadow_pmap  = new PhotonKDTreeNode( shadow_photons, shadow_aabb, 0 );
    caustic_pmap = new PhotonKDTreeNode( caustic_photons, caustic_aabb, 0 );
    // volume_pmap = new PhotonKDTreeNode( volume_photons, volume_aabb, 0 );
}

vec3 World::radiance( vec3 pt, Ray* ray, float dist, Object* obj,
                      size_t max_photons, Object* last_isect, int depth ) {
    vec3 rad =
        emitted_radiance( pt, obj ) +
        reflected_radance( pt, ray, dist, obj, max_photons, last_isect, depth );

    // std::cout << "rad // " << glm::to_string( rad ) << '\n' << endl;

    return rad;
}

vec3 World::emitted_radiance( vec3 pt, Object* obj ) {
    // HACK I think this is supposed to be whatever value is emitted at a point
    // (like on a lightbulb)

    for ( Light* light : lights ) {
        if ( SquareLight* sq_light = dynamic_cast<SquareLight*>( light ) ) {
            if ( sq_light->get_obj() == obj ) {
                return obj->get_material()->get_color();
            }
        }
    }

    return vec3( 0.0f );
}

vec3 World::reflected_radance( vec3 pt, Ray* ray, float dist, Object* obj,
                               size_t max_photons, Object* last_isect,
                               int depth ) {
    vec3 normal = obj->get_normal( ray, dist );
    vec3 radiance =
        direct_illumination( pt, obj, normal, max_photons ) +
        specular_reflection( pt, obj, ray, dist, last_isect, depth ) +
        caustics( pt, obj, normal, max_photons ) +
        multi_diffuse( pt, obj, normal, max_photons );
    return radiance;
}

vec3 World::direct_illumination( vec3 pt, Object* obj, vec3 norm,
                                 size_t max_photons ) {
    float radius           = 0.0f;
    vector<Photon*>* _heap = new vector<Photon*>();

    shadow_pmap->get_n_photons_near_pt( _heap, pt, norm, max_photons, &radius );

    PhotonHeap* heap = new PhotonHeap( _heap->begin(), _heap->end() );

    size_t shadows = 0;
    size_t count   = 0;

    while ( !heap->empty() && count < max_photons ) {
        Photon* p = heap->top();
        // radius = p->distance;
        if ( p->is_shadow ) {
            shadows += 1;
        }

        count++;
        heap->pop();
        delete p;
    }

    while ( !heap->empty() ) {
        Photon* p = heap->top();
        heap->pop();
        delete p;
    }

    delete heap;
    delete _heap;

    float divisor = ( 1 / ( 2 * M_PI * pow( radius, 2 ) ) );
    vec3 illum    = obj->get_material()->get_color() * divisor;

    // std::cout << "shadow:count // " << shadows << ':' << count << '\n';

    if ( shadows == count ) {
        // this spot is completely shadowed
        illum = vec3( 0.0f );
    } else if ( shadows == 0 ) {
        // we good
    } else {
        // this spot is partially shadowed
        float vis =
            static_cast<float>( count - shadows ) / static_cast<float>( count );
        illum = illum * vis;
    }

    // std::cout << "direct // " << glm::to_string( illum ) << '\n';

    return illum;
}

vec3 World::specular_reflection( vec3 pt, Object* obj, Ray* ray, float dist,
                                 Object* last_isect, int depth ) {
    vec3 spec = vec3( 0.0f );
    float kr  = obj->get_material()->get_kr();
    float kd  = obj->get_material()->get_kd();

    // if the object is reflective
    if ( kr > 0.0f ) {
        // spawn a reflection ray
        spec += calc_reflection( ray, pt, dist, obj, depth ) * kr;
    } else {
        // or don't bother
    }

    // if the object is transmissive
    if ( kd > 0.0f ) {
        // spawn a reflection ray
        spec += calc_refraction( ray, pt, dist, obj, last_isect, depth ) * kd;
    } else {
        // or don't bother
    }

    // std::cout << "specular // " << glm::to_string( spec ) << '\n';

    return spec;
}

vec3 World::caustics( vec3 pt, Object* obj, vec3 normal, size_t max_photons ) {
    vector<Photon*>* _heap    = new vector<Photon*>();
    IlluminationModel* imodel = obj->get_imodel();

    vec3 caustic = vec3( 0.0f );
    float radius = 0.0f;
    size_t count = 0;

    caustic_pmap->get_n_photons_near_pt( _heap, pt, normal, max_photons,
                                         &radius );

    PhotonHeap* heap = new PhotonHeap( _heap->begin(), _heap->end() );

    while ( !heap->empty() && count < max_photons ) {
        Photon* p = heap->top();

        Ray* source  = new Ray( &p->position, &p->dir );
        Ray* reflect = source->reflect( &normal );

        radius = p->distance;

        caustic +=
            imodel->get_specular( p->power, *reflect->direction, p->dir );

        delete source;
        delete reflect;

        count++;
        heap->pop();
        delete p;
    }

    while ( !heap->empty() ) {
        Photon* p = heap->top();
        heap->pop();
        delete p;
    }

    delete _heap;
    delete heap;

    float divisor = ( 1 / ( 2 * M_PI * pow( radius, 2 ) ) );

    caustic = caustic * divisor;

    // std::cout << "radius  // " << radius << '\n';
    // std::cout << "caustic // " << glm::to_string( caustic ) << '\n' << endl;

    return caustic;
}

vec3 World::multi_diffuse( vec3 pt, Object* obj, vec3 norm,
                           size_t max_photons ) {
    vector<Photon*>* _heap = new vector<Photon*>();

    IlluminationModel* imodel = obj->get_imodel();
    vec3 obj_col              = obj->get_material()->get_color();
    vec3 diffuse              = vec3( 0.0f );

    size_t count = 0;
    float radius = 0.0f;

    global_pmap->get_n_photons_near_pt( _heap, pt, norm, max_photons, &radius );

    PhotonHeap* heap = new PhotonHeap( _heap->begin(), _heap->end() );

    while ( !heap->empty() && count < max_photons ) {
        Photon* p = heap->top();

        diffuse += imodel->get_diffuse( p->power, obj_col, norm, p->dir ) +
                   p->power + p->power;
        radius = p->distance;

        count++;
        heap->pop();
        delete p;
    }

    while ( !heap->empty() ) {
        Photon* p = heap->top();
        heap->pop();
        delete p;
    }

    delete heap;
    delete _heap;

    float divisor = ( 1 / ( 2 * M_PI * pow( radius, 2 ) ) );

    diffuse = diffuse * divisor;

    // std::cout << "diffuse // " << glm::to_string( diffuse ) << '\n';

    return diffuse;
}

vec3 World::get_volumetric_light( Ray* r, float max_distance,
                                  float current_distance ) {
    // float step   = 0.1f;
    // size_t count = 0;
    //
    // if ( current_distance < max_distance ) {
    //     float radius;
    //     vector<Photon*>* _heap = new vector<Photon*>();
    //
    //     vec3 point = *r->origin + *r->direction * current_distance;
    //     volume_pmap->get_n_photons_near_pt( _heap, point, RAD_EST, &radius );
    //
    //     PhotonHeap* heap = new PhotonHeap( _heap->begin(), _heap->end() );
    //
    //     vec3 illum = vec3( 0.0f );
    //
    //     while ( !heap->empty() ) {
    //         Photon* p = heap->top();
    //
    //         if ( count++ < RAD_EST ) {
    //             illum += vec3( p->power );
    //         }
    //
    //         delete p;
    //     }
    //
    //     float divisor = ( 1 / ( 4 / 3 * M_PI * pow( radius, 3 ) ) );
    //
    //     illum = illum * divisor;
    //
    //     delete _heap;
    //     delete heap;
    //
    //     return illum +
    //            get_volumetric_light( r, max_distance, current_distance + step );
    // } else {
    return vec3( 0.0f );
    // }
}
