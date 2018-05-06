/**
 *
 */

#ifndef _SQUARELIGHT_H_
#define _SQUARELIGHT_H_

#include "Light.h"
#include "Rectangle.h"

class SquareLight : public Light {
  private:
    Rectangle* rect;
    vec3 normal;
    float max_angle;

    void set_proj_map();
    Photon* emit( float power );
    vec3 gen_emit_ori();
    vec3 gen_emit_dir();

  public:
    SquareLight( vec3 pos, vec3 normal, vec3 color, int photons, float width,
                 float length, float max_angle );
    ~SquareLight();

    std::vector<Photon*> emit_photons();

    // returns INT_MAX if given Ray does not intersect Object, or a value
    // indicating distance
    float intersection( Ray* ray );
    // transforms the object by a given transformation matrix
    void transform( mat4 matrix );

    Object* get_obj();
};

#endif
