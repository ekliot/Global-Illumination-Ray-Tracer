/**
 * World.h
 *
 * Object for a World scene, holding an object list for ray tracing
 *
 * Author: ekliot
 */

#ifndef _WORLD_H
#define _WORLD_H

#define _USE_MATH_DEFINES

#include <math.h>

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

    KDTreeNode* obj_tree;

    PhotonKDTreeNode* global_pmap;
    PhotonKDTreeNode* caustic_pmap;
    // PhotonKDTreeNode* volume_pmap;
    PhotonKDTreeNode* shadow_pmap;

    std::vector<Photon*> global_photons;
    std::vector<Photon*> caustic_photons;
    std::vector<Photon*> volume_photons;
    std::vector<Photon*> shadow_photons;

    vec3 background;
    vec3 ambient;
    float ir;

    const size_t MAX_DEPTH = 25;
    const size_t RAD_EST   = 100;

    std::vector<Object*> get_intersecting_objs( Ray* r, float dist );

    Object* get_intersected_obj( Ray* r, float* distance );

    std::vector<Light*> get_pruned_lights( vec3 point );

    // Light* adjusted_light_to_point( vec3 point, Light* light );
    bool can_see_light( vec3 point, Light* light );

    vector<Photon*> trim_photons( vector<Photon*> photons );
    void trace_photon( Photon* p, bool was_specular, bool diffused );

    void build_photon_maps();

    vec3 calc_refraction( Ray* ray, vec3 point, float dist, Object* intersect,
                          Object* last_isect, int depth );

    vec3 calc_reflection( Ray* ray, vec3 point, float dist, Object* intersect,
                          int depth );

    Object* get_intersect_kd_tree_helper( Ray* r, KDTreeNode* node,
                                          float* returnDist );

    vec3 radiance( vec3 pt, Ray* ray, float dist, Object* obj,
                   size_t max_photons, int depth );

    vec3 emitted_radiance( vec3 pt );

    vec3 reflected_radance( vec3 pt, Ray* ray, float dist, Object* obj,
                            size_t max_photons, int depth );

    vec3 direct_illumination( vec3 pt, Object* obj, Ray* r, float dist,
                              size_t max_photons );

    vec3 specular_reflection( vec3 pt, Ray* ray, float dist, Object* obj,
                              int depth );

    vec3 caustics( vec3 pt, size_t max_photons );

    vec3 multi_diffuse( vec3 pt, size_t max_photons );

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

    void emit_photons( size_t photon_count );

    /**
     * Returns the color a ray intersects in the scene
     *
     * @param r :: Ray :: a Ray spawned by the Camera that needs
     * intersection calculated
     *
     * @return :: vec3 :: the RGB value of the color intersected by a Ray
     */
    vec3 get_intersect( Ray* r, int depth = 0,
                        Object* lastIntersectionObject = NULL );

    Object* get_intersect_kd_tree( Ray* r, float* returnDist );

    void generate_kd_tree();

    void add_bunny();
};

#endif
