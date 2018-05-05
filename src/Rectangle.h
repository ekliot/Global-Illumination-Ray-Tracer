/**
 *
 */

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include <glm/vec3.hpp>
using glm::vec3;

#include "Triangle.h"

class Rectangle : public Object {

    Triangle* tri1;
    Triangle* tri2;

public:
    Rectangle( vec3 _ul, vec3 _ur, vec3 _lr, vec3 _ll, IlluminationModel* _imodel, Material* _mat );
    ~Rectangle();

    vec3 get_a();
    vec3 get_b();
    vec3 get_c();
    vec3 get_d();

    void transform( mat4 mat );
    float intersection( Ray* ray );
    vec3 get_normal( Ray* ray, float distance );

    AABB* getAABB();
    vec3 world_to_obj_space( vec3 point );
    vec2 get_uv( vec3 point );
};

#endif
