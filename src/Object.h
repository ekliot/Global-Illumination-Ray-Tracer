/**
 *
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm/matrix.hpp>

#include "Ray.h"
#include "IlluminationModel.h"

using namespace glm;

class Object {
protected:
    IlluminationModel* imodel;

public:
    Object( IlluminationModel* _imodel );

    virtual float intersection( Ray *ray ) = 0;

    virtual void transform( mat4 matrix ) = 0;

    IlluminationModel* get_imodel();

    vec3 convert( vec4 vector );
};

#endif
