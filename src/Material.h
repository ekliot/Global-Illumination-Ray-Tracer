/**
 *
 */

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <glm/vec3.hpp>
using glm::vec3;

class Material {

protected:
    float kr;
    float kd;

public:

    Material(float _kr, float _kd);
    // Material( const Material& material );
    virtual ~Material() = 0;
    // virtual Material& operator=( const Material& model ) = 0;

    virtual vec3 get_color( float u, float v ) = 0;

    float get_kr();
    float get_kd();
};

#endif
