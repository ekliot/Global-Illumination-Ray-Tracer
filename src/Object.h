#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm/matrix.hpp>

#include "Ray.h"
#include "Material.h"

using namespace glm;

class Object {
protected:
    Material* material;

public:
    Object( Material* mat );

    float intersection( Ray *ray );

    void transform( mat4 matrix );

    Material getMaterial();

    vec3 convert( vec4 vector );
};

#endif
