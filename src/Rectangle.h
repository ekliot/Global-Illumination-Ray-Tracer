/**
 *
 */

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include <glm/vec3.hpp>
using glm::vec3;

#include "Triangle.h"

class Rectangle : public Object {

    vec3* ul;
    vec3* ur;
    vec3* lr;
    vec3* ll;

    Triangle* tri1;
    Triangle* tri2;

    vec3 get_normal( Ray* ray, float distance );

    public:

        Rectangle( vec3* _ul, vec3* _ur, vec3* _lr, vec3* _ll, IlluminationModel* _imodel, Material* _mat );
        ~Rectangle();

        float intersection( Ray* ray );

        void transform( mat4 mat );

};

#endif
