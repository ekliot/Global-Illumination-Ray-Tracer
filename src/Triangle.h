#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

#include "Object.h"

using namespace glm;

class Triangle : public Object {

    vec3* a;
    vec3* b;
    vec3* c;

public:
    Triangle( vec3* _a, vec3* _b, vec3* _c, IlluminationModel* _imodel );

    float intersection( Ray* ray );

    vec3 get_normal( Ray* ray );

    void transform( mat4 matrix );
};

#endif // _TRIANGLE_H_
