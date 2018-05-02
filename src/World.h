/**
 * World.h
 *
 * Object for a World scene, holding an object list for ray tracing
 *
 * Author: ekliot
 */

#ifndef _WORLD_H
#define _WORLD_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

using namespace glm;

#include "Object.h"
#include "Ray.h"
#include "Light.h"

class World {

private:

    std::vector<Object*> objects;
    std::vector<Light*> lights;

    vec3 background;
    vec3 ambient;
    float ir;

    const int MAX_DEPTH = 20;

public:

    /**
     * Constructor
     */
    World( vec3 background, vec3 amb, float ir = 1.0 );

    /**
     * Destructor
     */
    ~World( void );

    /**
     * Adds an object to the scene
     */
    void add_object( Object* obj );

    /**
     *
     */
    void add_light( Light* light );

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
    vec3 get_intersect( Ray* r, mat4 inverse_transform_mat, int depth = 0, Object* lastIntersectionObject = NULL);


private:
    std::vector<Object*> get_intersecting_objs( Ray* r, float dist );
    Object* get_intersected_obj( Ray * r, float* distance );

    std::vector<Light> get_pruned_lights( vec3 point );

    Light adjusted_light_to_point( vec3 point, Light light );

    vec3 calc_refraction( Ray* ray, vec3 point, float dist, mat4 inv_trans_mat, Object* intersect, Object* last_isect, int depth );

};

#endif
