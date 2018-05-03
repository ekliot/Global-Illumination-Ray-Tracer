#ifndef _RAY_H_
#define _RAY_H_

#include <glm/vec3.hpp>

using namespace glm;

class Ray {
public:
    vec3* origin;
    vec3* direction;

    Ray( vec3* origin, vec3* dir );
    ~Ray();
    Ray* reflect(vec3* normal);
    void print();
};

#endif // _RAY_H_
