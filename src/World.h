/**
 * World.h
 *
 * Object for a World scene, holding an object list for ray tracing
 *
 * Author: ekliot
 */

#ifndef _WORLD_H
#define _WORLD_H
#include "../../stdafx.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <vector>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
using namespace glm;

#include "Object.h"
#include "Ray.h"

class World {
    
private:

    // TODO we'll need to make this into a spacial data struct // ekliot
    std::vector<Object> objects;

	vec4 background;

    // TODO wtf is this? it's in the UML, not sure what we need it for, commenting it out for now // ekliot
    // std::vector<Attribute> attributes;

    /**
     * Transforms an object in the scene in place
     *
     * @param obj :: Object :: the object to be transformed
     */
    void transform( Object obj );

public:

    /**
     * Constructor
     */
    World( vec4 background );

    /**
     * Destructor
     */
    ~World( void );

    /**
     * Adds an object to the scene
     */
    void add( Object obj );

    /**
     * Transforms all objects in the scene to camera coordinates
     *
     * @param tmat :: mat4 :: the transformation matrix to camera coordinates
     */
    void transform_all_to_ccoord( mat4 tmat );

    /**
     * Returns the colour a ray intersects in the scene
     *
     * @param r :: Ray :: a Ray spawned by the Camera that needs intersection calculated
     *
     * @return :: vec3 :: the RGB value of the colour intersected by a Ray
     */
    vec3 get_intersect( Ray r );

};

#endif
