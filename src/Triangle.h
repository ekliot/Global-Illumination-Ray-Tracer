#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#include "Object.h"
#include "Material.h"

using namespace glm;

class Triangle : public Object {

    vec3* a;
    vec3* b;
    vec3* c;

public:
    Triangle( vec3* a, vec3* b, vec3* c, Material* mat );

    float intersection( Ray* ray );

    void transform( mat4 matrix );
};

#endif // _TRIANGLE_H_
