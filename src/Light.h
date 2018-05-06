/**
 *
 */

#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <vector>

// FIXME THIS BREAKS EVERYTHING
// #include "Object.h"
#include "Photon.h"
#include "Ray.h"

using namespace glm;
using namespace photon;

class Light {
  protected:
    vec3 pos;
    vec3 color;
    int photons;

    virtual Photon* emit( float power ) = 0;

  public:
    Light( vec3 pos, vec3 color, int photons );
    virtual ~Light();

    virtual std::vector<Photon*> emit_photons() = 0;

    // returns INT_MAX if given Ray does not intersect Object, or a value
    // indicating distance
    virtual float intersection( Ray* ray ) = 0;
    // transforms the object by a given transformation matrix
    virtual void transform( mat4 matrix ) = 0;

    // virtual Object* get_obj() = 0;
    vec3 get_pos();
    vec3 get_color();
};

#endif
