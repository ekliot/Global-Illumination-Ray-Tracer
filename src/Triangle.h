#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>

#include "Object.h"

using namespace glm;

class Triangle : public Object {
    vec3 a;
    vec3 b;
    vec3 c;

  public:
    Triangle( vec3 _a, vec3 _b, vec3 _c, IlluminationModel* _imodel,
              Material* _mat );
    ~Triangle();

    vec3 get_a();
    vec3 get_b();
    vec3 get_c();

    void transform( mat4 matrix );
    float intersection( Ray* ray );
    vec3 get_normal( Ray* ray, float distance );

    AABB* get_aabb();

    vec3 world_to_obj_space( vec3 point );
    vec2 get_uv( vec3 point );
};

#endif  // _TRIANGLE_H_
