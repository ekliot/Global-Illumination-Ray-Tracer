/**
 * World.h
 *
 * Object for a World scene, holding an object list for ray tracing
 *
 * Author: ekliot
 */

#ifndef _WORLD_H
#define _WORLD_H

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "KDTreeNode.h"
#include "Light.h"
#include "Object.h"
#include "PhotonKDTreeNode.h"
#include "Ray.h"
#include "Triangle.h"

using namespace glm;

class World {
  private:

    std::vector<Object*> objects;
    std::vector<Light*> lights;

    KDTreeNode* objectTree;

    PhotonKDTreeNode* globalMap;
    PhotonKDTreeNode* causticMap;
    PhotonKDTreeNode* volumeMap;
    PhotonKDTreeNode* shadowMap;

    std::vector<Photon> globalPhotons;
    std::vector<Photon> causticPhotons;
    std::vector<Photon> volumePhotons;


    vec3 background;
    vec3 ambient;
    float ir;

    const int MAX_DEPTH = 25;

    std::vector<Object*> get_intersecting_objs( Ray* r, float dist );

    Object* get_intersected_obj( Ray* r, float* distance );

    std::vector<Light*> get_pruned_lights( vec3 point );

    // Light* adjusted_light_to_point( vec3 point, Light* light );
    bool can_see_light( vec3 point, Light* light );

    vec3 calc_refraction( Ray* ray, vec3 point, float dist, Object* intersect,
                          Object* last_isect, int depth );


    void trace_photon( Photon p, bool was_specular = false,
        bool diffused = false, bool shadow = false);

    Object* get_intersect_kd_tree( Ray* r, float* returnDist );

    Object* get_intersect_kd_tree_helper( Ray* r, KDTreeNode* node,
                                        float* returnDist);

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
     * @param r :: Ray :: a Ray spawned by the Camera that needs intersection
     * calculated
     *
     * @return :: vec3 :: the RGB value of the color intersected by a Ray
     */
    vec3 get_intersect( Ray* r, int depth = 0,
                        Object* lastIntersectionObject = NULL );

    void generate_kd_tree();

    void add_bunny();
};

#endif
