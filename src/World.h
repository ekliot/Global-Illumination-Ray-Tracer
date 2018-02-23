/**
 * World.h
 *
 * Object for a World scene, holding an object list for ray tracing
 *
 * Author: ekliot
 */

#ifndef _WORLD_H
#define _WORLD_H

#if defined(_WIN32) || defined(_WIN64)
#include "stdafx.h"
#include <windows.h>
#endif

#include <vector>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

using namespace glm;

#include "Object.h"
#include "Ray.h"
#include "Light.h"

class World {

private:

    // TODO we'll need to make this into a spacial data struct // ekliot
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    vec3 background;
    vec3 ambient;


    // TODO wtf is this? it's in the UML, not sure what we need it for, commenting it out for now // ekliot
    // std::vector<Attribute> attributes;

    /**
     * Transforms an object in the scene in place
     *
     * @param obj :: Object :: the object to be transformed
     */
    // void transform( Object obj );

public:

    /**
     * Constructor
     */
    World( vec3 background, vec3 amb );

    /**
     * Destructor
     */
    ~World( void );

    /**
     * Adds an object to the scene
     */
    void add_object( Object* obj );

    void add_light(Light* light);



    /**
     * Transforms all objects in the scene to camera coordinates
     *
     * @param tmat :: mat4 :: the transformation matrix to camera coordinates
     */
    void transform_all( mat4 tmat );

    /**
     * Returns the color a ray intersects in the scene
     *
     * @param r :: Ray :: a Ray spawned by the Camera that needs intersection calculated
     *
     * @return :: vec3 :: the RGB value of the color intersected by a Ray
     */
    vec3 get_intersect( Ray* r );

private:

    std::vector<Light*> pruned_lights(vec3 point);

    bool can_see_light(vec3 point, Light light);

};

#endif
