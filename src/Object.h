/**
 *
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm/matrix.hpp>

#include "Ray.h"
#include "Light.h"
#include "IlluminationModel.h"
#include "Texture.h"

using namespace glm;

class Object {
protected:
    IlluminationModel* imodel;
    Texture* texture;

public:
    Object( IlluminationModel* _imodel, Texture* _texture=NULL);
    virtual ~Object();

    virtual float intersection( Ray *ray ) = 0;

    virtual void transform( mat4 matrix ) = 0;

    virtual vec3 get_normal( Ray* ray, float distance ) = 0;

    vec2 get_uv( vec3* point);

    vec3 get_color( Ray* r, float dist, std::vector<Light*> lights, vec3* amb );

    vec3 convert( vec4* vector );
};

#endif
