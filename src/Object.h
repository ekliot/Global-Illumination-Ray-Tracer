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
    Object( IlluminationModel* _imodel, Material* _mat);
    virtual ~Object();

    // Object( const Object& model );
    // virtual Object& operator=( const Object& model ) = 0;

    virtual float intersection( Ray *ray ) = 0;

    virtual void transform( mat4 matrix ) = 0;

    virtual vec3 get_normal( Ray* ray, float distance ) = 0;

    vec2 get_uv( vec3* point);

    vec3 get_color( Ray* r, float dist, std::vector<Light*> lights, vec3* amb );

    vec3 convert( vec4* vector );

    virtual AABB* getAABB() = 0;

};

#endif
