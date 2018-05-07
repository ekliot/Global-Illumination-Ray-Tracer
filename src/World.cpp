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
    vec3 cur_color = radiance( position, ray, distance, obj, 100, depth );

    // IntersectData data;
    //
    // data.position = &position;
    //
    // vec3 normal = obj->get_normal( ray, distance );
    // data.normal = &normal;
    //
    // data.incoming = ray->direction;
    //
    // vec3 point  = *ray->origin + *ray->direction * distance;
    // data.lights = get_pruned_lights( point );
    //
    // data.ambient = &ambient;
    //
    // // the colour of the intersected object seen by the Ray
    // vec3 cur_color = obj->get_color( data );
    //
    // Object* l_obj;
    // for ( Light* l : lights ) {
    //     if ( SquareLight* sq_light = dynamic_cast<SquareLight*>( l ) ) {
    //         l_obj = sq_light->get_obj();
    //
    //         if ( l_obj == obj ) {
    //             cur_color = l_obj->get_material()->get_color();
    //         }
    //     }
    // }
    //
    // // if we have not hit maximum recursion depth...
    // if ( depth < MAX_DEPTH ) {
    //     float kd = obj->get_material()->get_kd();
    //     float kr = obj->get_material()->get_kr();
    //
    //     // if the object is transparent...
    //     if ( kd > 0 ) {
    //         vec3 refraction = calc_refraction( ray, point, distance, obj,
    //                                            last_intersect, depth );
    //         cur_color       = cur_color + kd * refraction;
    //     }
    //
    //     // if the object is reflective...
    //     if ( kr > 0 ) {
    //         vec3 reflection =
    //             calc_reflection( ray, point, distance, obj, depth );
    //         cur_color = cur_color + kr * reflection;
    //     }
    // } else {
    //     std::cout << "max recursion" << '\n';
    // }

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

void World::emit_photons( int photon_count ) {
    vector<Photon*> _photons;
    vector<Photon*> photons = vector<Photon*>();

    for ( Light* l : lights ) {
        _photons = l->emit_photons( static_cast<int>(
            photon_count / static_cast<int>( lights.size() ) ) );
        photons.insert( photons.end(), _photons.begin(), _photons.end() );
    }

    for ( Photon* p : photons ) {

        trace_photon( p, false, false );
        std::cout << "Photon:" << '\n';
        // std::cout << "\tpos // " << glm::to_string( p->position ) << '\n';
        // std::cout << "\tpow // " << glm::to_string( p->power ) << '\n';
        // std::cout << "\tdir // " << glm::to_string( p->dir ) << '\n';
        // std::cout << '\n';
    }

    build_photon_maps();
}

void World::trace_photon( Photon* p, bool was_specular, bool diffused, bool shadowed,
Object* lastIntersectionObject )
{
    Ray* r = new Ray( new vec3( p->position ), new vec3( p->dir ) );

    float distance        = 0;
    Object* intersect_obj = this->get_intersected_obj( r, &distance );

    if ( intersect_obj != NULL ) {

        p->position = vec3( p->position + p->dir * distance );

        vec3 normal_dir = intersect_obj->get_normal( r, distance );

        Ray* p_dir = r->reflect( &normal_dir );

        Photon* next_p   = new Photon();
        next_p->power    = vec3( p->power );
        next_p->position = vec3( p->position );
        next_p->dir      = vec3( *p_dir->direction );
        p->is_shadow     = false;

        //next_p->is_shadow = false;


        // Photon* shadowPhoton   = new Photon();
        // shadowPhoton->power    = vec3( p->power );
        // shadowPhoton->position = vec3( *p_dir->origin + *r->direction * 0.001f );
        // shadowPhoton->dir      = vec3( *r->direction );
        // //shadowPhoton->is_shadow = true;
        // delete shadowPhoton;
        // std::cout << gml:: glm::to_string(shadowPhoton->position) << "\n";


        if(!shadowed)
        {
            delete p_dir;

            float random =
                static_cast<float>( rand() ) / static_cast<float>( RAND_MAX );

            float kd = intersect_obj->get_imodel()->get_kd();
            float ks = intersect_obj->get_imodel()->get_ks();

            if ( random < kd ) {

                // std::cout << "diffuse!" << '\n';

                // diffuse
                if ( was_specular ) {
                    caustic_photons.push_back( p );
                }

                if ( diffused ) {
                    volume_photons.push_back( p );
                }

                global_photons.push_back( p );

                trace_photon( next_p, was_specular, true, shadowed, intersect_obj );
            } else if ( random < kd + ks ) {
                // std::cout << "specular!" << '\n';
                // specular reflection
                trace_photon( next_p, true, diffused, shadowed, intersect_obj );
            } else {
                // std::cout << "absorb!" << '\n';
                // absorption
            }
            if(lastIntersectionObject == NULL)
            {
                shadow_photons.push_back(p);
            }
        }
        else
        {
            // p->is_shadow = true;
            // shadow_photons.push_back(p);
        }
        // trace_photon(shadowPhoton, was_specular, diffused, true, intersect_obj);


    }

    delete r;
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
                      size_t max_photons, int depth ) {
    vec3 rad = emitted_radiance( pt ) +
               reflected_radance( pt, ray, dist, obj, max_photons, depth );

    // std::cout << "rad // " << glm::to_string( rad ) << '\n' << endl;

    return rad;
}

vec3 World::emitted_radiance( vec3 pt ) {
    // HACK I think this is supposed to be whatever value is emitted at a point
    // (like on a lightbulb)
    return vec3( 0.0f );
}

vec3 World::reflected_radance( vec3 pt, Ray* ray, float dist, Object* obj,
                               size_t max_photons, int depth ) {
    vec3 radiance = direct_illumination( pt, obj, ray, dist, max_photons ) +
                    // specular_reflection( pt, ray, dist, obj, depth ) +
                    // caustics( pt, max_photons ) +
                    multi_diffuse( pt, max_photons );
    return radiance;
}

vec3 World::direct_illumination( vec3 pt, Object* obj, Ray* r, float dist,
                                 size_t max_photons ) {
    photon::PhotonHeap* heap = new PhotonHeap();
    vec3 obj_col             = obj->get_material()->get_color();
    vec3 illum               = vec3( 0.0f );
    vec3 norm                = obj->get_normal( r, dist );

    float radius;

    shadow_pmap->get_n_photons_near_pt( heap, pt, max_photons, &radius );

    size_t shadows = 0;
    size_t count = 0;

    while ( !heap->empty() ) {
        Photon* p = heap->top();

        if(count++ < max_photons)
        {
            illum +=
                obj->get_imodel()->get_diffuse( p->power, obj_col, norm, p->dir );

            if ( p->is_shadow ) {
                shadows += 1;
            }
        }

        delete p;



        heap->pop();
    }

    if ( shadows == 0 ) {
        // this spot is directly illuminated
        // keep it as it is
    } else if ( shadows == max_photons ) {
        // this spot is completely chadowed
        illum = ambient;
    } else {
        // this spot is partially chadowed
        float vis = ( max_photons - shadows ) / max_photons;
        illum     =  obj_col * vis;
    }

    //std::cout << "direct // " << glm::to_string( illum ) << '\n';
    delete heap;

    return illum * 50.0f;
}

vec3 World::specular_reflection( vec3 pt, Ray* ray, float dist, Object* obj,
                                 int depth ) {
    float kr = obj->get_material()->get_kr();

    // if the object is reflective
    if ( kr > 0.0f ) {
        // spawn a reflection ray
        return calc_reflection( ray, pt, dist, obj, depth ) * kr;
    } else {
        // or don't bother
        return vec3( 0.0f );
    }
}

vec3 World::caustics( vec3 pt, size_t max_photons ) {
    // photon::PhotonHeap* heap = new PhotonHeap();
    //
    // // float radius = 0.0f;
    // vec3 caustic = vec3( 0.0f );
    //
    // caustic_pmap->get_n_photons_near_pt( heap, pt, max_photons );
    //
    // while ( !heap->empty() ) {
    //     Photon* p = heap->top();
    //
    //     // HACK how does BRDF come into play here?
    //     caustic += p->power;
    //     //delete p;
    //     // radius = p->distance;
    //
    //     heap->pop();
    // }
    //
    // // double divisor = ( 1 / ( M_PI * pow( radius, 2 ) ) );
    // double divisor = 1.0f;
    //
    // caustic =
    //     vec3( caustic.x * divisor, caustic.y * divisor, caustic.z * divisor
    //     );
    //
    // std::cout << "caustic // " << glm::to_string( caustic ) << '\n';
    //
    // delete heap;
    //
    // return caustic;
    return vec3( 0, 0, 0 );
}

vec3 World::multi_diffuse( vec3 pt, size_t max_photons ) {
    photon::PhotonHeap* heap = new PhotonHeap();

    vec3 diffuse = vec3( 0.0f );
    float radius = 0.0f;

    global_pmap->get_n_photons_near_pt( heap, pt, max_photons, &radius );
    //std::cout << "made diff map";
    //std::cout << "heap size: " << heap->size() << '\n';
    size_t count = 0;

    while ( !heap->empty() ) {
        Photon* p = heap->top();

        // print check if the photon is bogus
        if ( p->power.x > 1.0f || p->power.y > 1.0f || p->power.z > 1.0f ||
             p->power.x < 0.0f || p->power.y < 0.0f || p->power.z < 0.0f ) {
            std::cout << "Photon:" << '\n';
            std::cout << "\tpos  // " << glm::to_string( p->position ) << '\n';
            std::cout << "\tpow  // " << glm::to_string( p->power ) << '\n';
            std::cout << "\tdir  // " << glm::to_string( p->dir ) << '\n';
            std::cout << "\tsrc  // " << glm::to_string( p->src ) << '\n';
            std::cout << "\tdist // " << p->distance << '\n';
            std::cout << '\n';
        }
        //HACK how does BRDF come into play here?
        if(count ++ < max_photons)
            diffuse += p->power;
        // radius = p->distance;
        delete p;
        heap->pop();
    }
    //std::cout << "radius // " << radius << '\n';

    float divisor = 50.0f * ( 1 / ( M_PI * pow( radius, 2 ) ) );
    //double divisor = 1.0f;

    diffuse = vec3( diffuse ) * divisor;

    //std::cout << "diffuse // " << glm::to_string( diffuse ) << '\n';

    delete heap;

    return diffuse;
}
