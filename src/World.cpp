/**
 * World.cpp
 */

#include <algorithm>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstring>
#include <ctime>
#include <fstream>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <queue>
#include <sstream>
#include <thread>
#include <vector>

#include "glm/gtx/string_cast.hpp"
#include "tinyply.h"

using namespace glm;
using namespace tinyply;

#include "Light.h"
#include "Phong.h"
#include "SolidMaterial.h"
#include "World.h"

World::World( vec3 bg, vec3 amb, float _ir )
    : background( bg ), ambient( amb ), ir( _ir ) {}

World::~World() {
    for ( Object* o : objects ) {
        delete o;
    }

    for ( Light* l : lights ) {
        delete l->color;
        delete l->position;
    }
}

void World::add_object( Object* obj ) { objects.push_back( obj ); }

void World::add_light( Light* light ) { lights.push_back( light ); }

void World::transform_all( mat4 tmat ) {
    for ( Object* obj : objects ) {
        obj->transform( tmat );
    }
    for ( Light* light : lights ) {
        vec4 _pos        = vec4( light->position->x, light->position->y,
                          light->position->z, 1 );
        vec4 newPosition = tmat * _pos;

        light->position->x = newPosition.x;
        light->position->y = newPosition.y;
        light->position->z = newPosition.z;
        // vec3 Object::convert( vec4* vec ) {
        //     return vec3( vec->x / vec->w, vec->y / vec->w, vec->z / vec->w );
        // }
    }
}

std::vector<Light> World::get_pruned_lights( vec3 point ) {
    std::vector<Light> ret_lights;

    for ( size_t i = 0; i < lights.size(); i++ ) {
        Light adj_light = adjusted_light_to_point( point, *lights[i] );
        if ( *( adj_light.color ) != vec3( -1.0f ) ) {
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

// returns a light with color RGBs of -1.0f if the given Light is not visible
// from the given point
Light World::adjusted_light_to_point( vec3 point, Light light ) {
    // vec3 _dir = point - *light.position;
    vec3 _dir = *light.position - point;

    float dist = length( _dir );
    // a Ray pointing from the given point to the given light
    Ray r = Ray( &point, &_dir );

    // a list of objects between the point and Light, sorted by
    // closest->farthest from the Light
    std::vector<Object*> isecting_objs = get_intersecting_objs( &r, dist );

    // deep copy the original light
    vec3* adj_col = new vec3( light.color->x, light.color->y, light.color->z );
    vec3* adj_pos =
        new vec3( light.position->x, light.position->y, light.position->z );

    bool visible = true;
    float kd     = 0.0f;
    vec3 _adj    = vec3( 0.0f );

    // for every object between the light and point, modify the colour of the
    // light reaching that point based on the object colours and transparencies
    for ( Object* obj : isecting_objs ) {
        kd = obj->get_material()->get_kd();

        visible = visible && ( kd > 0.0f );
        if ( !visible ) {
            adj_col = new vec3( -1.0f );
            break;
        } else {
            // std::cout << "adjusting light..." << '\n';
            //
            // std::cout << "  cur_col // " << glm::to_string(*adj_col) << '\n';
            // std::cout << "  obj_col // " <<
            // glm::to_string(obj->get_material()->get_color( 0.0f, 0.0f )) <<
            // '\n'; std::cout << "  kd // " << kd << '\n';

            // TODO it'd be cool to make this alter the light's colour based on
            // the objet colour... this has failed in the past, however perhaps,
            // if we model light absorption? IDEA light_col - ( vec(1.0f) -
            // obj_col ) * ( 1.0f - kd ) _adj = normalize( *adj_col +
            // obj->get_material()->get_color( 0.0f, 0.0f ) ) * kd;

            _adj       = *adj_col * ( kd );
            adj_col->x = _adj.x;
            adj_col->y = _adj.y;
            adj_col->z = _adj.z;

            // if ( *adj_col != vec3( -1.0f ) && *adj_col != *light.color ) {
            //     std::cout << "got color: " << glm::to_string( *adj_col ) <<
            //     '\n'; std::cout << "from:      " << glm::to_string(
            //     *light.color ) << '\n';
            // }
        }
    }

    Light ret = { adj_col, adj_pos };

    return ret;
}

std::vector<Object*> World::get_intersecting_objs( Ray* r, float disty ) {
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

        if ( _dist < INT_MAX && _dist > 0.00001 && _dist < disty ) {
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

    return ret_obj;
}

vec3* World::get_intersect_kd_tree_helper( Ray* r, KDTreeNode* node,
                                           float* returnDist,
                                           mat4 inverse_transform_mat ) {
    float paramReturnDist = FLT_MAX;

    if ( node->left == NULL ) {
        *returnDist = INT_MAX;

        for ( Object* obj : node->objects ) {
            float newValue = obj->intersection( r );
            if ( newValue < *returnDist && newValue > 0.00001 ) {
                *returnDist = newValue;
                vec3 value  = vec3( 0 );
                // vec3 value = obj->get_color( &*r, *returnDist, &lights,
                // &ambient, inverse_transform_mat );

                // vec3 cur_color = intersect_obj->get_color( ray, distance,
                // ret_lights, &ambient, inverse_transform_mat );

                return new vec3( value );
            }
        }
    }
    if ( node->left == NULL ) {
        return NULL;
    }

    float a_enter = node->left->aabb->intersect_ray( r );
    // float a_exit = INT_MAX;
    // if( a_enter != INT_MAX)
    // {
    //     vec3 newOrigin = *r->origin + ( a_enter+.01f  * *r->direction ) ;
    //     Ray enterRay = Ray(&newOrigin, r->direction);
    //     a_exit = node->left->aabb->intersect_ray(&enterRay);
    //
    // }

    float b_enter = node->right->aabb->intersect_ray( r );
    // float b_exit = INT_MAX;
    // if( b_enter != INT_MAX)
    // {
    //     vec3 newOrigin = *r->origin + ( b_enter+.01f * *r->direction) ;
    //     Ray enterRay = Ray(&newOrigin, r->direction);
    //     a_exit = node->right->aabb->intersect_ray(&enterRay);
    // }

    if ( a_enter < 0 ) a_enter = INT_MAX;

    if ( b_enter < 0 ) b_enter = INT_MAX;

    if ( a_enter == INT_MAX && b_enter == INT_MAX ) {
        return NULL;
    }

    if ( abs( a_enter - b_enter ) < 0.00001 ) {
        float a_dist;
        vec3* a_vec = get_intersect_kd_tree_helper( r, node->left, &a_dist,
                                                    inverse_transform_mat );

        float b_dist;

        vec3* b_vec = get_intersect_kd_tree_helper( r, node->right, &b_dist,
                                                    inverse_transform_mat );
        if ( a_dist < b_dist ) {
            return a_vec;
        } else {
            return b_vec;
        }

    } else if ( a_enter <= b_enter && a_enter != INT_MAX ) {
        vec3* a_vec = get_intersect_kd_tree_helper(
            r, node->left, &paramReturnDist, inverse_transform_mat );
        if ( a_vec != NULL ) {
            return a_vec;
        }
        vec3* b_vec = get_intersect_kd_tree_helper(
            r, node->right, &paramReturnDist, inverse_transform_mat );
        if ( b_enter != INT_MAX ) {
            return b_vec;
        }
    } else if ( b_enter < a_enter && b_enter != INT_MAX ) {
        vec3* b_vec = get_intersect_kd_tree_helper(
            r, node->right, &paramReturnDist, inverse_transform_mat );
        if ( b_vec != NULL ) {
            return b_vec;
        }
        vec3* a_vec = get_intersect_kd_tree_helper(
            r, node->left, &paramReturnDist, inverse_transform_mat );
        if ( a_enter != INT_MAX ) {
            return a_vec;
        }
    }

    *returnDist = FLT_MAX;
    return NULL;
}

vec3 World::get_intersect_kd_tree( Ray* r, mat4 inverse_transform_mat ) {
    float returnValue = FLT_MAX;
    vec3* color = get_intersect_kd_tree_helper( r, objectTree, &returnValue,
                                                inverse_transform_mat );
    if ( color != NULL ) {
        return *color;
    } else {
        return background;
    }

    return vec3( 0, 0, 0 );
}

// return a the colour seen by a given Ray at its point of intersection
vec3 World::get_intersect( Ray* ray, mat4 inverse_transform_mat, int depth,
                           Object* last_intersect ) {
    float distance        = 0;
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
    vec3 point                    = *ray->origin + *ray->direction * distance;
    std::vector<Light> ret_lights = get_pruned_lights( point );

    // the colour of the intersected object seen by the Ray
    vec3 cur_color = intersect_obj->get_color(
        ray, distance, ret_lights, &ambient, inverse_transform_mat );

    // if we have not hit maximum recursion depth...
    if ( depth < MAX_DEPTH ) {
        float kd = intersect_obj->get_material()->get_kd();
        float kr = intersect_obj->get_material()->get_kr();

        // if the object is transparent...
        if ( kd > 0 ) {
            vec3 refract_color =
                calc_refraction( ray, point, distance, inverse_transform_mat,
                                 intersect_obj, last_intersect, depth );
            cur_color = cur_color + kd * refract_color;
        }

        // if the object is reflective...
        if ( kr > 0 ) {
            // std::cout << "reflectin..." << '\n';

            vec3 normal_dir = intersect_obj->get_normal( ray, distance );

            vec3 reflect_dir = glm::reflect( *ray->direction, normal_dir );
            vec3 newPoint    = point + .1f * reflect_dir;

            Ray reflect_ray = Ray( &newPoint, &reflect_dir );

            // std::cout << "reflect ray:" << '\n';
            // reflect_ray.print();
            // std::cout << '\n';

            vec3 reflect_color =
                get_intersect( &reflect_ray, inverse_transform_mat, depth + 1 );
            cur_color = cur_color + kr * reflect_color;
        }
    } else {
        std::cout << "max recursion" << '\n';
    }

    return cur_color;
}

vec3 World::calc_refraction( Ray* ray, vec3 point, float dist,
                             mat4 inv_trans_mat, Object* intersect,
                             Object* last_isect, int depth ) {
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

    return get_intersect( &refract_ray, inv_trans_mat, depth + 1, intersect );
}

void World::generate_kd_tree() {
    AABB* currentAABB =
        new AABB( -FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX );
    for ( Object* obj : objects ) {
        currentAABB = new AABB( currentAABB, obj->getAABB() );
    }
    objectTree = new KDTreeNode( objects, currentAABB, 0 );
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

void World::buildProtonKDTree( std::vector<Photon> photons ) {
    vec3 maxVec = vec3( FLT_MIN );
    vec3 minVec = vec3( FLT_MAX );

    for ( Photon photon : photons ) {
        maxVec.x = std::max( maxVec.x, photon.position.x );
        maxVec.y = std::max( maxVec.y, photon.position.y );
        maxVec.z = std::max( maxVec.z, photon.position.z );

        minVec.x = std::min( minVec.x, photon.position.x );
        minVec.y = std::min( minVec.y, photon.position.y );
        minVec.z = std::min( minVec.z, photon.position.z );
    }
    AABB* currentAABB = new AABB( maxVec, minVec );

    photonKDTree = new PhotonKDTreeNode( photons, currentAABB, 0 );
}

std::priority_queue<Photon, std::vector<Photon>, World::compare>
World::getPhotons( vec3 position, float range ) {
    std::priority_queue<Photon, std::vector<Photon>, compare> returnPhotons;
    return returnPhotons;
}

void World::getPhotonHelper(
    std::priority_queue<Photon, std::vector<Photon>, World::compare>* photons,
    PhotonKDTreeNode node, vec3 position, float range ) {
    if ( node.left != NULL ) {
        float delta = 0;

        // calculate distance
        if ( node.axis == 0 ) {
            delta = node.left->aabb->get_max().x - position.x;
        } else if ( node.axis == 1 ) {
            delta = node.left->aabb->get_max().y - position.y;
        } else if ( node.axis == 2 ) {
            delta = node.left->aabb->get_max().z - position.z;
        }

        if ( delta < 0 ) {
            getPhotonHelper( photons, *node.left, position, range );
            if ( pow( delta, 2 ) > pow( range, 2 ) ) {
                getPhotonHelper( photons, *node.right, position, range );
            }
        } else {
            getPhotonHelper( photons, *node.right, position, range );
            if ( pow( delta, 2 ) > pow( range, 2 ) ) {
                getPhotonHelper( photons, *node.left, position, range );
            }
        }
    } else {
        for ( size_t i = 0; i < node.photons.size(); i++ ) {
            Photon oldPhoton = node.photons.at( i );
            Photon newPhoton = {};

            newPhoton.position = vec3( oldPhoton.position );
            newPhoton.power    = vec3( oldPhoton.power );
            newPhoton.phi      = oldPhoton.phi;
            newPhoton.theta    = oldPhoton.theta;
            newPhoton.flag     = oldPhoton.flag;
            newPhoton.distance = glm::distance( newPhoton.position, position );

            photons->push( newPhoton );
        }
    }
}
