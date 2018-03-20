#ifndef _PLANE_H_
#define _PLANE_H_

#include <glm/vec3.hpp>
using glm::vec3;

#include "Object.h"
#include "Triangle.h"

class Rectangle : public Object {

    vec3* ll;
    vec3* lr;
    vec3* ul;
    vec3* ur;

    Triangle* tri1;
    Triangle* tri2;

    vec3 get_normal( Ray* ray, float distance );

    public:

        Rectangle(vec3* _ll, vec3* _lr, vec3* _ul, vec3* _ur, IlluminationModel* _imodel );

        float intersection( Ray* ray );

        void transform( mat4 mat );

};

#endif
