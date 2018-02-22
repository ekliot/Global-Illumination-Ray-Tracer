#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <glm/matrix.hpp>

#include "Ray.h"
#include "Material.h"
#include "Light.h"

using namespace glm;

class Object {
protected:
    Material* material;

public:
    Object( Material* mat );

    virtual float intersection( Ray *ray ) = 0;

    virtual void transform( mat4 matrix ) = 0;

    virtual vec3 get_normal(Ray* ray) = 0;

    vec4 get_color(Ray ray, float distance, std::vector<Light> lights);

    vec3 convert( vec4 vector );
};

#endif
