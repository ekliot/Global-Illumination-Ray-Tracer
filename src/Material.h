/**
 *
 */

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <glm/vec3.hpp>
using glm::vec3;

class Material {
public:
    Material();
    // Material( const Material& material );
    virtual ~Material() = 0;
    // virtual Material& operator=( const Material& model ) = 0;

    virtual vec3 get_color( float u, float v ) = 0;
};

#endif
