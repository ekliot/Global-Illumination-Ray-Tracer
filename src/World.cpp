/**
 * World.cpp
 *
 * Author: ekliot
 */

#include <climits>
#include <iostream>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cmath>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include "glm/gtx/string_cast.hpp"
#include "tinyply.h"

using namespace glm;
using namespace tinyply;


#include "World.h"
#include "Light.h"
#include "Phong.h"
#include "SolidMaterial.h"

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



vec3* World::get_intersect_kd_tree_helper( Ray *r, KDTreeNode* node, float* returnDist)
{

    float paramReturnDist = FLT_MAX;


    if(node->left == NULL)
    {

        *returnDist = INT_MAX;

        for ( Object* obj : *node->objects ) {

            float newValue = obj->intersection( r );
            if ( newValue < *returnDist && newValue > 0.00001 )
            {

                *returnDist = newValue;

                vec3 value = obj->get_color( r, *returnDist, lights, &ambient );
                return new vec3(value);
            }
        }
    }
    if(node->left == NULL)
    {
        return NULL;
    }

    float a_enter = node->left->aabb->intersectRay(r);
    // float a_exit = INT_MAX;
    // if( a_enter != INT_MAX)
    // {
    //     vec3 newOrigin = *r->origin + ( a_enter+.01f  * *r->direction ) ;
    //     Ray enterRay = Ray(&newOrigin, r->direction);
    //     a_exit = node->left->aabb->intersectRay(&enterRay);
    //
    // }

    float b_enter = node->right->aabb->intersectRay(r);
    // float b_exit = INT_MAX;
    // if( b_enter != INT_MAX)
    // {
    //     vec3 newOrigin = *r->origin + ( b_enter+.01f * *r->direction) ;
    //     Ray enterRay = Ray(&newOrigin, r->direction);
    //     a_exit = node->right->aabb->intersectRay(&enterRay);
    // }


    if(a_enter < 0)
        a_enter = INT_MAX;

    if(b_enter < 0)
        b_enter = INT_MAX;

    if(a_enter == INT_MAX && b_enter == INT_MAX)
    {
        return NULL;
    }

    if ( abs(a_enter - b_enter) < 0.00001) {

        return NULL;


    }
    else if(a_enter <= b_enter && a_enter != INT_MAX)
    {
        vec3 *a_vec = get_intersect_kd_tree_helper(r, node->left, &paramReturnDist);
        if(a_vec != NULL)
        {
            return a_vec;
        }
        vec3 *b_vec = get_intersect_kd_tree_helper(r, node->right, &paramReturnDist);
        if(b_enter != INT_MAX)
        {
            return b_vec;
        }
    }
    else if(b_enter < a_enter && b_enter != INT_MAX)
    {

        vec3 *b_vec = get_intersect_kd_tree_helper(r, node->right, &paramReturnDist);
        if(b_vec != NULL)
        {
            return b_vec;
        }
        vec3 *a_vec = get_intersect_kd_tree_helper(r, node->left, &paramReturnDist);
        if(a_enter != INT_MAX)
        {
            return a_vec;
        }
    }


    *returnDist = FLT_MAX;
    return NULL;
}


vec3 World::get_intersect_kd_tree( Ray *r )
{
    float returnValue =  FLT_MAX;
    vec3* color = get_intersect_kd_tree_helper(r,objectTree, &returnValue);
    if(color != NULL)
    {
        return *color;
    }
    else
    {
        return background;
    }

    return vec3(0,0,0);
}

vec3 World::get_intersect( Ray *r  ) {

    float distance = 0;
    Object* intersectObject = this->get_intersect_helper( r, &distance );

    // TODO SPLIT THIS SHIT

    if ( intersectObject != NULL ) {
        // do work to do things
        vec3 point = *r->origin + *r->direction * distance;
        std::vector<Light*> returnLights = pruned_lights( point );
        return intersectObject->get_color( r, distance, returnLights, &ambient );
    }

    return background;
}

void World::generate_kd_tree()
{
    AABB* currentAABB = new AABB(-FLT_MAX, -FLT_MAX, -FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
    for ( Object* obj : objects ) {
        currentAABB = new AABB(currentAABB, obj->getAABB());
    }
//    currentAABB->print();
    objectTree = new KDTreeNode(objects, currentAABB, 0);
}

void World::add_bunny()
{
    try
    {
        const std::string filename = "src/tinyply/bunny.ply";
        // Read the file and create a std::istringstream suitable
        // for the lib -- tinyply does not perform any file i/o.
        std::ifstream ss(filename, std::ios::binary);

        if (ss.fail())
        {
            throw std::runtime_error("failed to open ");
        }

        PlyFile file;
        std::shared_ptr<PlyData> vertices, faces;

        file.parse_header(ss);

        try { vertices = file.request_properties_from_element("vertex", { "x", "y", "z" }); }
        catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }

        try { faces = file.request_properties_from_element("face", { "vertex_indices" }); }
        catch (const std::exception & e) { std::cerr << "tinyply exception: " << e.what() << std::endl; }


        file.read(ss);

        const size_t numVerticesBytes = vertices->buffer.size_bytes();

        struct float3 { float x, y, z; };
        std::vector<float3> verts(vertices->count);
        std::memcpy(verts.data(), vertices->buffer.get(), numVerticesBytes);

        const size_t numFacesBytes = faces->buffer.size_bytes();
        struct int3 { uint x, y, z; };
        std::vector<int3> triangles(faces->count);
        std::memcpy(triangles.data(), faces->buffer.get(), numFacesBytes);

        std::vector<Triangle*> tris;

        for (size_t count = 0; triangles.size() > count; count++) {
            float3 firstVert = verts[triangles[count].x];
            vec3 firstVec = vec3(firstVert.x, firstVert.y, firstVert.z) + vec3( 0.77f, 2.7f, -5.0f );

            float3 secondVert = verts[triangles[count].y];
            vec3 secondVec = vec3(secondVert.x, secondVert.y, secondVert.z) + vec3( 0.77f, 2.7f, -5.0f );

            float3 thirdVert = verts[triangles[count].z];
            vec3 thirdVec = vec3(thirdVert.x, thirdVert.y, thirdVert.z) + vec3( 0.77f, 2.7f, -5.0f );

            Phong* bunny_imodel = new Phong(
                vec3( 1.0f, 1.0f, 1.0f ),
                // ka,  kd,   ks,   ke
                0.1f, 0.5f, 0.1f, 20.0f
            );
            SolidMaterial* bunny_mat = new SolidMaterial( vec3( 0.5f, 0.0f, 0.0f ) );


            //add triangle here
            Triangle* triangle = new Triangle(&firstVec, &secondVec, &thirdVec, bunny_imodel, bunny_mat);
            add_object(triangle);
            //return_objects->push_back(triangle);
            //tris.push_back(triangle);

            //add triangle to the world here
        }
    }
    catch (const std::exception & e)
    {
        std::cerr << "Caught tinyply exception: " << e.what() << std::endl;
    }

}
