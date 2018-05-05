/**
 *
 */

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "Triangle.h"

class Rectangle : public Object {
    Triangle* tri1;
    Triangle* tri2;

  public:
    Rectangle( vec3 a, vec3 b, vec3 c, vec3 d, IlluminationModel* _imodel,
               Material* _mat );
    ~Rectangle();

    vec3 get_a();
    vec3 get_b();
    vec3 get_c();
    vec3 get_d();

    void transform( mat4 mat );
    float intersection( Ray* ray );
    vec3 get_normal( Ray* ray, float distance );

    AABB* get_aabb();
    vec3 world_to_obj_space( vec3 point );
    vec2 get_uv( vec3 point );
};

#endif
