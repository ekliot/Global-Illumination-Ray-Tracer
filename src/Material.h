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
    float ir;

  public:
    Material( float kr = 0.0f, float kd = 0.0f, float ir = 0.0f );
    // Material( const Material& material );
    virtual ~Material();
    // virtual Material& operator=( const Material& model ) = 0;

    virtual vec3 get_color( float u = 0, float v = 0 ) = 0;

    float get_kr();
    float get_kd();
    float get_ir();
};

#endif
