/**
 *
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm/matrix.hpp>

#include "Ray.h"
#include "Light.h"
#include "IlluminationModel.h"
#include "AABB.h"
#include "Material.h"

using namespace glm;

class Object {
protected:
    IlluminationModel* imodel;
    Material* material;

    float minHelper(float oldValue, float newValue);
    float maxHelper(float oldValue, float newValue);

    vec3 minVecHelper(vec3 oldValue, vec3 newValue);
    vec3 maxVecHelper(vec3 oldValue, vec3 newValue);

public:
    Object( IlluminationModel* _imodel, Material* _mat );
    virtual ~Object();

    // Object( const Object& model );
    // virtual Object& operator=( const Object& model ) = 0;

    // returns a 3D vector of the RGB value of a point on the object intersected by a given Ray
    vec3 get_color( Ray* r, float dist, std::vector<Light> lights, vec3* amb, mat4 rev_tmat );
    // reduces a 4D vector to a 3D vector
    vec3 convert( vec4 vector );

    //gets the object's material
    Material* get_material();

    // returns INT_MAX if given Ray does not intersect Object, or a value indicating distance
    virtual float intersection( Ray *ray ) = 0;
    // transforms the object by a given transformation matrix
    virtual void transform( mat4 matrix ) = 0;
    // returns the normal for a point of intersection
    virtual vec3 get_normal( Ray* ray, float distance ) = 0;

    virtual vec3 world_to_obj_space( vec3 point );
    // vec2 get_uv( vec3 point );
    virtual vec2 get_uv( vec3 point );

    virtual AABB* getAABB() = 0;

};

#endif
